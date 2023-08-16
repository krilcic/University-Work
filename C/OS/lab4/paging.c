#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <windows.h>

int N;
int M;
int t;
unsigned char ***disk;
unsigned char **mem;
unsigned short **translation_table;

int fetch_phys_adress(int p, int x)
{
    int up = (x >> 6) & 0x0F;
    int down = x & 0x3F;
    int translate = translation_table[p][up];
    int presence = (translate >> 5) & 0x1;
    if (presence == 0)
    {
        printf("Miss!\n");
        int count = 0;
        int frame;
        for (int i = 0; i < N; i++)
        {
            for (int j = 0; j < 15; j++)
            {
                int temp = translation_table[i][j];
                if (((temp >> 5) & 0x1) == 1)
                {
                    count++;
                }
            }
        }
        if ((((translation_table[0][15]) >> 5) & 0x1) == 1)
        {
            count++;
        }
        if (count < M)
        {
            frame = count;
        }
        else
        {
            if ((((translation_table[0][15]) >> 5) & 0x1) == 0)
            {
                int index_i;
                int index_j;
                int mini = 32;

                for (int i = 0; i < N; i++)
                {
                    for (int j = 0; j < 16; j++)
                    {
                        int temp = translation_table[i][j];
                        if (((temp & 0x1F) < mini) && (((temp >> 5) & 0x1) == 1))
                        {
                            index_i = i;
                            index_j = j;
                            mini = temp & 0x1F;
                        }
                    }
                }

                frame = (translation_table[index_i][index_j] >> 6) & 0x3FF;

                translation_table[index_i][index_j] = translation_table[index_i][index_j] & 0xFFDF;

                for (int i = 0; i < 64; i++)
                {
                    disk[index_i][index_j][i] = mem[frame][i];
                }

                int out;
                out = (index_j << 6) & 0x3C0;
                printf("Throwing out page 0x%04x from process %d\n", out, index_i);

                int lru_out;
                lru_out = translation_table[index_i][index_j] & 0x1F;
                printf("lru of thrown out page: 0x%04x\n", lru_out);
            }
            else
            {
                printf("throwing out the frame of shared container\n");
                frame = (translation_table[p][15] >> 6) & 0x3FF;
                for (int i = 0; i < N; i++)
                {
                    translation_table[i][15] = translation_table[i][15] & 0xFFDF;
                }
                for (int i = 0; i < 64; i++)
                {
                    disk[0][15][i] = mem[frame][i];
                }
            }
        }

        printf("assigned frame: 0x%04x\n", frame);

        int translate2 = frame;
        translate2 = (translate2 << 6) | 0x00;
        translate2 = translate2 | 0x20;
        if (t == 31)
        {
            for (int i = 0; i < N; i++)
            {
                for (int j = 0; j < 16; j++)
                {
                    translation_table[i][j] = translation_table[i][j] & 0xFFE0;
                }
            }
            translate2 = translate2 | 1;
        }
        else
        {
            translate2 = translate2 | t;
        }
        translation_table[p][up] = translate2;

        if (up < 15)
        {
            for (int i = 0; i < 64; i++)
            {
                mem[frame][i] = disk[p][up][i];
            }
        }
        else
        {
            for (int i = 0; i < 64; i++)
            {
                mem[frame][i] = disk[0][15][i];
            }
            for (int i = 0; i < N; i++)
            {
                translation_table[i][15] = translation_table[i][15] | 0x20;
            }
        }
    }

    int physical;
    physical = translation_table[p][up] & 0xFFC0;
    physical = physical | down;

    return physical;
}

unsigned short fetch_content(int p, int x)
{
    int physical = fetch_phys_adress(p, x);
    int gore = (physical >> 6) & 0x0F;
    int dole = physical & 0x3F;

    unsigned short a = 0;
    a = mem[gore][dole];
    a = a + (mem[gore][dole + 1] << 8);
    if (x < 960)
    {
        printf("physical adress: 0x%04x\n", physical);

        int gore2 = (x >> 6) & 0x0F;

        printf("Table record: 0x%04x\n", translation_table[p][gore2]);
        printf("Content of the adress: %d\n", mem[gore][dole]);
    }
    return a;
}

void write_value(int p, int x, unsigned short i)
{
    int physical = fetch_phys_adress(p, x);
    int up = (physical >> 6) & 0x0F;
    int down = physical & 0x3F;

    mem[up][down] = i & 0x00FF;
    mem[up][down + 1] = (i & 0xFF00) >> 8;
}

int main(int argc, char *argv[])
{
    N = atoi(argv[1]);
    M = atoi(argv[2]);
    t = 0;

    disk = (unsigned char ***)malloc(N * sizeof(unsigned char **));
    for (int i = 0; i < N; i++)
    {
        disk[i] = (unsigned char **)malloc(16 * sizeof(unsigned char *));
        for (int j = 0; j < 16; j++)
        {
            disk[i][j] = (unsigned char *)malloc(64 * sizeof(unsigned char));
        }
    }

    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < 16; j++)
        {
            for (int k = 0; k < 64; k++)
            {
                disk[i][j][k] = 0;
            }
        }
    }

    mem = (unsigned char **)malloc(M * sizeof(unsigned char *));
    for (int i = 0; i < M; i++)
    {
        mem[i] = (unsigned char *)malloc(64 * sizeof(unsigned char));
    }

    for (int i = 0; i < M; i++)
    {
        for (int j = 0; j < 64; j++)
        {
            mem[i][j] = 0;
        }
    }

    translation_table = (unsigned short **)malloc(N * sizeof(unsigned short *));
    for (int i = 0; i < N; i++)
    {
        translation_table[i] = (unsigned short *)malloc(16 * sizeof(unsigned short));
    }

    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < 16; j++)
        {
            translation_table[i][j] = 0;
        }
    }

    srand(time(NULL));

    while (1)
    {
        for (int i = 0; i < N; i++)
        {
            printf("Process: %d\n", i);
            printf("t: %d\n", t);
            if (i == 0)
            {
                for (int j = 0; j < N - 1; j++)
                {
                    unsigned short poruka = rand() % 9999;
                    write_value(i, 1000 + 2 * j, poruka);
                    printf("Sent message: %d\n", poruka);
                }
            }
            else
            {
                unsigned short poruka = fetch_content(i, 1000 + 2 * (i - 1));
                printf("Received message: %d\n", poruka);
            }
            int x = 510; // rand() %959
            // printf("log. address: 0x%04x\n",x);
            unsigned short value = fetch_content(i, x);
            value++;
            write_value(i, x, value);
            if (t == 31)
            {
                t = 0;
            }
            t++;
            Sleep(100);
        }
    }

    return 0;
}