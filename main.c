#include <stdio.h>
#include <ctype.h>

enum State
{
    NORMAL,
    SINGLE_QUOTED,
    DOUBLE_QUOTED,
    COMMENTED,
};

enum State handle_normal(int input, int *line_count, int *comment_line)
{
    enum State state = NORMAL;
    if (input == '"')
    {
        state = DOUBLE_QUOTED;
    }
    else if (input == '\'')
    {
        state = SINGLE_QUOTED;
    }
    else if (input == '/')
    {
        int next_char = getchar();
        if (next_char == '\n')
            (*line_count) += 1;
        else if (next_char == '*')
        {
            *comment_line = *line_count;
            state = COMMENTED;
            putchar(' ');
        }
        else
        {
            putchar(input);
            input = next_char;
        }
    }

    if (state != COMMENTED)
        putchar(input);
    return state;
}

enum State handle_single_quoted(int input)
{
    putchar(input);
    return (input == '\'') ? NORMAL : SINGLE_QUOTED;
}

enum State handle_double_quoted(int input)
{
    putchar(input);
    return (input == '"') ? NORMAL : DOUBLE_QUOTED;
}

enum State handle_commented(int input, int *line_count)
{
    if (input == '\n')
        putchar(input);

    else if (input == '*')
    {
        int next_char = getchar();
        if (next_char == '\n')
            (*line_count) += 1;

        else if (next_char == '/')
            return NORMAL;
    }
    return COMMENTED;
}

int main()
{
    int input;
    enum State state = NORMAL;
    int line_count = 1, comment_begin = 0;

    while ((input = getchar()) != EOF)
    {
        if (input == '\n')
            ++line_count;

        switch (state)
        {
        case NORMAL:
            state = handle_normal(input, &line_count, &comment_begin);
            break;
        case SINGLE_QUOTED:
            state = handle_single_quoted(input);
            break;
        case DOUBLE_QUOTED:
            state = handle_double_quoted(input);
            break;
        case COMMENTED:
            state = handle_commented(input, &line_count);
            break;
        }
    }

    if (state == COMMENTED)
    {
        fprintf(stderr, "Error: line %d: unterminated comment\n", comment_begin);
        return -1;
    }

    return 0;
}