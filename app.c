#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

int main()
{
    char input = ' ';
    char output = ' ';
    int newlines = 0;
    int ttt = 0;

    FILE *test  = fopen("/proc/my_info", "r");   //cout
    while ((output = fgetc(test)) != EOF)
    {
        if(output == '\n')
            ttt++;
    }
    //printf("%d\n\n\n\n\n",ttt);

    int count = ttt - 15;
    fclose(test);


    printf("Which information do you want?\n");
    printf("Version(v),CPU(c),Memory(m),Time(t),All(a),Exit(e)?\n");
    while(input != 'e')
    {
        FILE *in_file  = fopen("/proc/my_info", "r"); // read only
        // test for files not existing.
        if (in_file == NULL)
        {
            printf("Error! Could not open file\n");
            exit(-1); // must include stdlib.h
        }
        scanf("%c",&input);
        switch(input)
        {
        case 'v':
            while ((output = fgetc(in_file)) != EOF)
            {
                if (output == '\n')
                {
                    newlines++;
                    if (newlines == 4 )
                    {
                        newlines = 0;
                        printf("\n----------------------------------------------------------\n\n");
                        break;
                    }
                }
                printf("%c",output);
            }
            printf("Which information do you want?\n");
            printf("Version(v),CPU(c),Memory(m),Time(t),All(a),Exit(e)?\n");
            fclose(in_file);
            break;
        case 'c':
            while ((output = fgetc(in_file)) != EOF)
            {
                if (output == '\n')
                {
                    newlines++;
                    if (newlines == count)    // 16
                    {
                        newlines = 0;
                        printf("\n----------------------------------------------------------\n\n");
                        break;
                    }
                }
                if(newlines >= 4)
                    printf("%c",output);
            }
            printf("Which information do you want?\n");
            printf("Version(v),CPU(c),Memory(m),Time(t),All(a),Exit(e)?\n");
            fclose(in_file);
            break;

        case 'm':
            while ((output = fgetc(in_file)) != EOF)
            {
                if (output == '\n')
                {
                    newlines++;
                    if (newlines == (count+12)) //28
                    {
                        newlines = 0;
                        printf("\n----------------------------------------------------------\n\n");
                        break;
                    }
                }
                if(newlines >= count)  //16
                    printf("%c",output);
            }
            printf("Which information do you want?\n");
            printf("Version(v),CPU(c),Memory(m),Time(t),All(a),Exit(e)?\n");
            fclose(in_file);
            break;
        case 't':
            while ((output = fgetc(in_file)) != EOF)
            {
                if (output == '\n')
                {
                    newlines++;
                    if (newlines == (count+12+3) )     //31
                    {
                        newlines = 0;
                        printf("\n\n----------------------------------------------------------\n\n");
                        break;
                    }
                }
                if(newlines >= (count+12))   //28
                    printf("%c",output);
            }
            printf("Which information do you want?\n");
            printf("Version(v),CPU(c),Memory(m),Time(t),All(a),Exit(e)?\n");
            fclose(in_file);
            break;
        case 'a':
            output = fgetc(in_file);
            while (output != EOF)
            {
                printf ("%c", output);
                output = fgetc(in_file);
            }
            printf("\n----------------------------------------------------------\n\n");
            fclose(in_file);
            printf("Which information do you want?\n");
            printf("Version(v),CPU(c),Memory(m),Time(t),All(a),Exit(e)?\n");
            break;
        case 'e':
            return 0;
        default:
            break;

        }
    }

    return 0;
}
