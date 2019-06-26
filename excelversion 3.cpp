// excel.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include<string.h>
#include<stdlib.h>
struct  node
{
	int flag;
	int value;
	char *s;
};
int isoperator(char ch)
{
	char oper[5] = { '+', '-', '*', '/', '\0' };
	int i = 0;
	while (oper[i] != '\0')
	{
		if (oper[i] == ch)
			return 1;
		i++;
	}
	return 0;

}
int ischar(char ch)
{
	if (ch >= 65 && ch <= 91)
		return 1;
	return 0;
}
int eval(int in1, int in2, char op)
{
	if (op == '+')
		return in1 + in2;
	if (op == '-')
		return in1 - in2;
	if (op == '*')
		return in1*in2;
	if (op == '/')
		return in1 / in2;
}
int valueof(char s[10])
{
	int j = 0, num = 0;
	while (s[j] != '\0')
	{
		num = num * 10 + (s[j] - '0');
		j++;
	}
	return num;
}
int isnum(char arg[10], int index)
{
	int i;
	for (i = index; arg[i] != '\0'; i++)
		if (arg[i] < 48 || arg[i]>57)
			return 0;
	return 1;
}
int expeval(char arg[2],struct node * sheet[10][10])
{
	int i = 0, j, pos,col,row,flag1=0,flag2=0;
	col = arg[0] - 'A';
	row = (arg[1] - '0') - 1;
	char op1[4], op2[4],str[20];
	if (sheet[row][col]->flag == 0)return sheet[row][col]->value;
	strcpy_s(str, sheet[row][col]->s);
	while (str[i] != '\0')
	{
		j = isoperator(str[i]);
		if (j == 1)
			break;
		i++;
	}
	if (str[i] != '\0')
		pos = i;
	else
	{
		for (i = 0; str[i] != '\0'; i++)
			op1[i] = str[i];
		op1[i] = '\0';
		return expeval(op1, sheet);
	}

	for (i = 0; i < pos; i++)
		op1[i] = str[i];
	op1[i] = '\0';
	for (j = 1; str[j + pos] != '\0'; j++)
		op2[j - 1] = str[j + pos];
	op2[j - 1] = '\0';
	flag1 = isnum(op1,0);
	flag2 = isnum(op2, 0);
	if (flag1==1&&flag2==1)
	   return eval(valueof(op1), valueof(op2), str[pos]);
	if (flag1==1)
		return eval(valueof(op1), expeval(op2,sheet), str[pos]);
	if (flag2==1)
		return eval(expeval(op1, sheet), valueof(op2), str[pos]);
	return eval(expeval(op1, sheet), expeval(op2, sheet), str[pos]);

}

void get(char arg[2], struct node * sheet[10][10])
{
		printf("%d", expeval(arg,sheet));
}

void set(char arg[10], struct node * sheet[10][10])
{
	int row, col, value = 0, index, pos = 0;
	col = arg[0] - 'A';
	row = (arg[1] - '0') - 1;
	index = 3;
	if (isnum(arg, index) == 1)
	{
		while (arg[index] != '\0')
		{
			value = value * 10 + (arg[index] - '0');
			index++;
		}
		sheet[row][col]->value = value;
	}
	else
	{
		sheet[row][col]->flag = 1;
		while (arg[index] != '\0')
		{
			sheet[row][col]->s[pos++] = arg[index];
			index++;
		}
		sheet[row][col]->s[pos] = '\0';
	}
}
void print(struct node * sheet[10][10])
{
	int i, j;
	char arg[2];
	for (i = 0; i < 10; i++)
		printf("___");
	printf("\n");
	for (i = 0; i < 10; i++)
	{
		for (j = 0; j < 10; j++)
		{
			arg[0] = j + 'A';
			arg[1] = i + '1';
			
			printf("|%d|", expeval(arg, sheet));
		}
		printf("\n");
		for (j = 0; j < 10; j++)
			printf("___");
		printf("\n");
	}
}

void exportfun(char arg[100], struct node *sheet[10][10])
{
	FILE *fptr;
	int i, j, k, index = 0, num, pos = 0;
	fopen_s(&fptr, arg, "w");
	char ch[100], rev[4];
	for (i = 0; i < 10; i++)
	{
		k = 0;
		for (j = 0; j < 10; j++)
		{
			if (sheet[i][j]->flag == 0)
			{
				num = sheet[i][j]->value;
				if (num == 0)
				{
					ch[k++] = '0';
					ch[k++] = ',';
					continue;
				}
				pos = 0;
				while (num != 0)
				{
					rev[pos++] = '0' + (num % 10);
					num /= 10;
				}
				rev[pos] = '\0';
				for (pos = pos - 1; pos >= 0; pos--)
					ch[k++] = rev[pos];
			}
			else
			{
				index = 0;
				while (sheet[i][j]->s[index] != '\0')
				{
					ch[k++] = sheet[i][j]->s[index];
					index++;
				}
			}
			ch[k++] = ',';
		}
		ch[k - 1] = '\0';
		fprintf(fptr, "%s\n", ch);
	}
	fclose(fptr);
}
int tonum(char str[30])
{
	int index = 0, value = 0;
	while (str[index] != '\0')
	{
		value = value * 10 + (str[index] - '0');
		index++;
	}
	return value;
}
void import(char arg[100], struct node *sheet[10][10])
{
	FILE *fptr;
	int i, j, k, index;
	fopen_s(&fptr, arg, "r");
	char *ch = (char *)malloc(sizeof(char) * 20);
	char temp[20];
	i = 0;
	while (fscanf(fptr, "%s", ch) != EOF)
	{
		index = 0; j = 0; k = 0;
		while (ch[index] != '\0')
		{
			if (ch[index] != ',')
				temp[k++] = ch[index];
			else
			{
				temp[k] = '\0';
				if (isnum(temp, 0))
				{
					sheet[i][j]->flag = 0;
					sheet[i][j]->value = tonum(temp);
				}
				else
				{
					sheet[i][j]->flag = 1;
					strcpy(sheet[i][j]->s, temp);
				}
				j++; k = 0;
			}
			index++;
		}

		fseek(fptr, 1, SEEK_CUR); i++;

	}

	fclose(fptr);
}
void get_command(char cmd[255], struct node * sheet[10][10], char *cur_file)
{
	int index, i, argpos, j = 0;
	cmd[strcspn(cmd, "\n")] = '\0';
	char arg[20];
	if (!strcmp(cmd, "PRINT"))
	{
		print(sheet);
		return;
	}
	if (!strcmp(cmd, "SAVE"))
	{
		exportfun(cur_file, sheet);
		return;
	}
	index = strcspn(cmd, " ");
	i = index;
	while (cmd[i] != '\0'&&cmd[i] == ' ')
	{
		i++;
	}
	if (cmd[i] != '\0')
	{
		argpos = i;
		while (cmd[argpos] != '\0')
		{
			arg[j++] = cmd[argpos];
			argpos++;
		}
		arg[j] = '\0';
	}
	cmd[index] = '\0';
	if (!strcmp(cmd, "GET"))
		get(arg, sheet);
	else if (!strcmp(cmd, "SET"))
		set(arg, sheet);
	else if (!strcmp(cmd, "PRINT"))
		print(sheet);
	else if (!strcmp(cmd, "EXPORT"))
		exportfun(arg, sheet);
	else if (!strcmp(cmd, "IMPORT"))
	{
		strcpy(cur_file, arg);
		import(arg, sheet);
	}
	else
		printf("INVALID COMMAND");
}
int main()
{
	struct node *sheet[10][10];
	char *cur_file = (char *)malloc(sizeof(char) * 20);
	cur_file[0] = '\0';
	int i, j;
	for (i = 0; i < 10; i++)
	{
		for (j = 0; j < 10; j++)
		{
			sheet[i][j] = (struct node *)malloc(sizeof(struct node));
			sheet[i][j]->flag = 0;
			sheet[i][j]->value = 0;
			sheet[i][j]->s = (char *)malloc(sizeof(char) * 20);
		}
	}
	char cmd[255];
	do
	{
		printf("\n>");
		fgets(cmd, 255, stdin);
		get_command(cmd, sheet, cur_file);
	} while (strcmp(cmd, "exit"));
	return 0;
}

