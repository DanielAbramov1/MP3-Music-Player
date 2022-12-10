// Daniel Abramov #204621627

#define _CRT_SECURE_NO_WARNINGS //printf and scanf are not secure - the secure way is to write printf_s and scanf_s (we are using basic c with scanf and printf)
#include<stdio.h>
#include<string.h>
#include<stdlib.h>

typedef struct node // struct of song type
{
	char id[6];
	char art_name[31];
	char album_name[31];
	char track_num[10];
	char track_name[31];
	struct node* prev;
	struct node* next;
}node;

node* create_song(char id[], char art_name[], char album_name[], char track_num[], char track_name[])//making a node out of inputs
{
	node* data;
	data = (node*)malloc(sizeof(node));
	strcpy(data->id, id);
	strcpy(data->art_name, art_name);
	strcpy(data->album_name, album_name);
	strcpy(data->track_num, track_num);
	strcpy(data->track_name, track_name);
	data->next = NULL;
	data->prev = NULL;
	return data;
}

void importData(FILE* path) //print the fisrt init of the mp3 
{
	char id[6], artistName[31], albumName[31], trackNo[10], trackName[31];	//note +1 for '\0'
	char line[110];
	FILE* fp = fopen(path, "r");
	FILE* log_file = fopen("log_file.txt","a");
	if (!fp)
	{
		printf("File not found!\n");
		return;
	}
	while (fgets(line, 110, fp) > 0)
	{
		strncpy(id, line, 5);
		id[5] = '\0';
		strncpy(artistName, line + 6, 30);
		artistName[30] = '\0';
		strncpy(albumName, line + 37, 30);
		albumName[30] = '\0';
		strncpy(trackNo, line + 67, 9);
		trackNo[9] = '\0';
		strncpy(trackName, line + 77, 30);
		trackName[30] = '\0';
		printf("%s%s%s%s%s\n", id, artistName, albumName, trackNo, trackName);
		fprintf(log_file, "%s%s%s%s%s\n", id, artistName, albumName, trackNo, trackName);
	}
	fclose(log_file);
	fclose(fp);
	return;
}

node* sorting_file_to_list(FILE* path)// making linked list and returning the head pointer with full list
{
	char id[6], art_name[31], album_name[31], track_num[10], track_name[31];
	char line[110];
	node* head, * temp_song, * temp_pt;
	FILE* open_file = fopen(path, "r");
	if (!open_file)
	{
		printf("File not found!\n");
		return NULL;
	}
	fgets(line, 110, open_file);
	strncpy(id, line, 5);
	id[5] = '\0';
	strncpy(art_name, line + 6, 30);
	art_name[30] = '\0';
	strncpy(album_name , line + 37, 30);
	album_name[30] = '\0';
	strncpy(track_num, line + 67, 9);
	track_num[9] = '\0';
	strncpy(track_name, line + 77, 30);
	track_name[30] = '\0';	
	head = create_song(id, art_name, album_name, track_num, track_name); // header of the file
	temp_pt = head;
	while (fgets(line, 110, open_file) > 0)
	{
		strncpy(id, line, 5);
		id[5] = '\0';
		strncpy(art_name, line + 6, 30);
		art_name[30] = '\0';
		strncpy(album_name, line + 37, 30);
		album_name[30] = '\0';
		strncpy(track_num, line + 67, 9);
		track_num[9] = '\0';
		strncpy(track_name, line + 77, 30);
		track_name[30] = '\0';
		temp_song = create_song(id, art_name, album_name, track_num, track_name);
		temp_pt->next = temp_song;
		temp_song->prev = temp_pt;
		temp_pt = temp_song;
	}
	fclose(open_file);
	return head;
}

char* BufferingSpaces(char org[], int len)
{
	int i=0,j;
	char* temp = (char*)malloc(sizeof(char*) * len);
	while (org[i] != '\0')
	{
		temp[i] = org[i];
		i++;
	}
	for (j = i; j < len; j++)
	{
		if (temp[j] <= 0)
    	temp[j] = ' ';
	}
	temp[len - 1] = '\0';
	return temp;
}

node* NextSong(node* head, node* current_song)//head is for the last song of the play list
{
	if (current_song->next == NULL)
		current_song = head->next;
	else
		current_song = current_song->next;
	return current_song;
}

node* PrevSong(node* head, node* current_song)//head here is for skipping the tittle in file
{
	current_song = current_song->prev;
	if (head->id == current_song->id)
	{
		while (current_song->next != NULL)
			current_song = current_song->next;
	}
	return current_song;
}

void SearchSong(node* head)
{
	node* temp1, * temp2;
	char name[31],buffer=" ";
	int search_by = 0;
	FILE* log_file = fopen("log_file.txt", "a");
	char c = '\n';
	temp1 = head->next;
	temp2 = head;
	printf("Please choose the search option:\n(1) - Artist Name\n(2) - Track Name\n(3) - Album name\n");
	fprintf(log_file,"Please choose the search option:\n(1) - Artist Name\n(2) - Track Name\n(3) - Album name\n");
	scanf("%d", &search_by);
	fprintf(log_file, "%d\n", search_by);
	if (search_by == 1)
	{
		printf("Please enter Artist name:\n");
		fprintf(log_file,"Please enter Artist name:\n");
		buffer = getchar();
		scanf("%[^\n]s", name);
		strcpy(name, BufferingSpaces(name, 31));
		fprintf(log_file, "%s\n", name);
		printf("Search resaults are:\n");
		fprintf(log_file, "Search resaults are:\n");
		while (temp1->next != NULL)
		{
			if (strcmp(temp1->art_name, name) == 0)
			{
				printf("%s%s%s%s%s\n", temp1->id, temp1->art_name, temp1->album_name, temp1->track_name, temp1->track_num);
				fprintf(log_file,"%s%s%s%s%s\n", temp1->id, temp1->art_name, temp1->album_name, temp1->track_name, temp1->track_num);
			}
			temp1 = temp1->next;
		}
	}
	if (search_by == 2)
	{
		printf("Please enter Track name:\n");
		fprintf(log_file, "Please enter Track name:\n");
		buffer = getchar();
		scanf("%[^\n]s", name);
		strcpy(name, BufferingSpaces(name, 31));
		fprintf(log_file, "%s\n", name);
		printf("Search resaults are:\n");
		fprintf(log_file, "Search resaults are:\n");
		while (temp1->next != NULL)
		{
			if (strcmp(temp1->track_name, name) == 0)
			{
				printf("%s%s%s%s%s\n", temp1->id, temp1->art_name, temp1->album_name, temp1->track_name, temp1->track_num);
				fprintf(log_file, "%s%s%s%s%s\n", temp1->id, temp1->art_name, temp1->album_name, temp1->track_name, temp1->track_num);
			}
			temp1 = temp1->next;
		}
	}
	if (search_by == 3)
	{
		printf("Please enter Album name:\n");
		fprintf(log_file, "Please enter Album name:\n");
		buffer = getchar();
		scanf("%[^\n]s", name);
		strcpy(name, BufferingSpaces(name, 31));
		fprintf(log_file, "%s\n", name);
		printf("Search resaults are:\n");
		fprintf(log_file,"Search resaults are:\n");
		while (temp1->next != NULL)
		{
			if (strcmp(temp1->album_name, name) == 0)
			{
				printf("%s%s%s%s%s\n", temp1->id, temp1->art_name, temp1->album_name, temp1->track_name, temp1->track_num);
				fprintf(log_file, "%s%s%s%s%s\n", temp1->id, temp1->art_name, temp1->album_name, temp1->track_name, temp1->track_num);
			}
			temp1 = temp1->next;
		}
	}
	fclose(log_file);
}

void DeleteSong(node* head)
{
	node* temp1, * temp2, *to_delete;
	char id[6];
	int i;
	FILE* log_file = fopen("log_file.txt", "a");
	temp1 = head;
	printf("You selected to delete a song fron the playlist\nPlease enter the ID of the song you want to delete:\n");
	fprintf(log_file,"You selected to delete a song fron the playlist\nPlease enter the ID of the song you want to delete:\n");
	scanf("%s",id);
	strcpy(id, BufferingSpaces(id, 6));
	fprintf(log_file, "%s", id);
	while (strcmp(temp1->id, id) != 0)
	{
		if (temp1->next == NULL)
		{
			printf("No song found!\n");
			return;
		}
		temp1 = temp1->next;
	}
	to_delete = temp1;
	temp2 = temp1->prev;
	temp2->next = temp1->next;
	temp1 = temp2->next;
	temp1->prev = temp2;
	free(to_delete);
	fclose(log_file);
}

void InsertSong(node* head)
{
	node* temp, * new_song;
	char id[6], art_name[31], album_name[31], track_num[10], track_name[31];
	char buffer=" ";
	FILE* log_file = fopen("log_file.txt", "a");
	printf("You selected to insert a song to the play list\nPlease enter the next fields:\n");
	fprintf(log_file,"You selected to insert a song to the play list\nPlease enter the next fields:\n");
	printf("Please enter the ID:\n");
	fprintf(log_file,"Please enter the ID:\n");
	scanf("%s", id);
	strcpy(id, BufferingSpaces(id, 6));
	fprintf(log_file, "%s\n", id);
	printf("Please enter the Artist Name:\n");
	fprintf(log_file,"Please enter the Artist Name:\n");
	buffer = getchar();
	scanf("%[^\n]s", art_name);
	strcpy(art_name, BufferingSpaces(art_name, 31));
	fprintf(log_file, "%s\n", art_name);
	printf("Please enter the Album Name:\n");
	fprintf(log_file,"Please enter the Album Name:\n");
	buffer = getchar();
	scanf("%[^\n]s", album_name);
	strcpy(album_name, BufferingSpaces(album_name, 31));
	fprintf(log_file, "%s\n", album_name);
	printf("Please enter the Track Number:\n");
	fprintf(log_file,"Please enter the Track Number:\n");
	buffer = getchar();
	scanf("%[^\n]s", track_num);
	strcpy(track_num, BufferingSpaces(track_num, 10));
	fprintf(log_file, "%s\n", track_num);
	printf("Please enter the Track Name:\n");
	fprintf(log_file,"Please enter the Track Name:\n");
	buffer = getchar();
	scanf("%[^\n]s", track_name);
	strcpy(track_name, BufferingSpaces(track_name, 31));
	fprintf(log_file, "%s\n", track_name);
	temp = head;
	while (temp->next != NULL)
		temp = temp->next;
	new_song = create_song(id, art_name, album_name, track_num, track_name);
	temp->next = new_song;
	new_song->prev = temp;
	fclose(log_file);
}

int WordsCheck(char current[], char next[])
{
	for (int i = 0; i < 31; i++)
	{
		if (tolower(current[i]) > tolower(next[i]))
			return 1;
		if (tolower(current[i]) < tolower(next[i]))
			return 0;
	}
	return 0;
}

void SortBy(node* head)
{
	node*current, *index;
	char temp_id[6], temp_art_name[31], temp_album_name[31], temp_track_num[10], temp_track_name[31];
	int sort_by = 0;
	FILE* log_file = fopen("log_file.txt", "a");
	if ((head == NULL)||(head->next==NULL))
	{
		printf("The playlist is empty!\n");
		return;
	}
	printf("Please choose the sort option:\n(1) - Artist Name\n(2) - Track Name\n(3) - Album name\n");
	fprintf(log_file,"Please choose the sort option:\n(1) - Artist Name\n(2) - Track Name\n(3) - Album name\n");
	scanf("%d", &sort_by);
	fprintf(log_file, "%d\n", sort_by);
	fclose(log_file);
	if (sort_by == 1)
	{
		for (current = head->next; current->next != NULL; current = current->next)
		{
			for (index = current->next; index != NULL; index = index->next)
			{
				if (WordsCheck(current->art_name, index->art_name))
				{
					strcpy(temp_id, current->id);
					strcpy(temp_art_name, current->art_name);
					strcpy(temp_album_name, current->album_name);
					strcpy(temp_track_num, current->track_num);
					strcpy(temp_track_name, current->track_name);

					strcpy(current->id, index->id);
					strcpy(current->art_name, index->art_name);
					strcpy(current->album_name, index->album_name);
					strcpy(current->track_num, index->track_num);
					strcpy(current->track_name, index->track_name);

					strcpy(index->id, temp_id);
					strcpy(index->art_name, temp_art_name);
					strcpy(index->album_name, temp_album_name);
					strcpy(index->track_num, temp_track_num);
					strcpy(index->track_name, temp_track_name);
				}
			}
		}
	}
	if (sort_by == 2)
	{
		for (current = head->next; current->next != NULL; current = current->next)
		{
			for (index = current->next; index != NULL; index = index->next)
			{
				if (WordsCheck(current->track_name, index->track_name))
				{
					strcpy(temp_id, current->id);
					strcpy(temp_art_name, current->art_name);
					strcpy(temp_album_name, current->album_name);
					strcpy(temp_track_num, current->track_num);
					strcpy(temp_track_name, current->track_name);

					strcpy(current->id, index->id);
					strcpy(current->art_name, index->art_name);
					strcpy(current->album_name, index->album_name);
					strcpy(current->track_num, index->track_num);
					strcpy(current->track_name, index->track_name);

					strcpy(index->id, temp_id);
					strcpy(index->art_name, temp_art_name);
					strcpy(index->album_name, temp_album_name);
					strcpy(index->track_num, temp_track_num);
					strcpy(index->track_name, temp_track_name);
				}
			}
		}
	}
	if(sort_by==3)
	{ 
		for (current = head->next; current->next != NULL; current = current->next)
		{
			for (index = current->next; index != NULL; index = index->next)
			{ 
				if (WordsCheck(current->album_name, index->album_name))
				{
					strcpy(temp_id, current->id);
					strcpy(temp_art_name , current->art_name);
					strcpy(temp_album_name, current->album_name);
					strcpy(temp_track_num, current->track_num);
					strcpy(temp_track_name, current->track_name);

					strcpy(current->id, index->id);
					strcpy(current->art_name, index->art_name);
					strcpy(current->album_name, index->album_name);
					strcpy(current->track_num, index->track_num);
					strcpy(current->track_name, index->track_name);
					
					strcpy(index->id, temp_id);
					strcpy(index->art_name, temp_art_name);
					strcpy(index->album_name, temp_album_name);
					strcpy(index->track_num, temp_track_num);
					strcpy(index->track_name, temp_track_name);
				}
			}
		}
	}
}

void SaveAndExit(char* new_plalist,node* head)
{
	node* temp;
	char c = ' ';
	FILE* path = fopen(new_plalist, "w");
	FILE* log_file = fopen("log_file.txt", "a");
	if (!path)
	{
		printf("File not found!\n");
		return;
	}
	temp = head;
	printf("\nThis is the updated playlist:\n");
	fprintf(log_file,"\nThis is the updated playlist:\n");
	while (temp!=NULL)
	{
		printf("%s%s%s%s%s\n", temp->id, temp->art_name, temp->album_name, temp->track_num, temp->track_name);
		fprintf(log_file,"%s%s%s%s%s\n", temp->id, temp->art_name, temp->album_name, temp->track_num, temp->track_name);
		temp = temp->next;
	}
	while (head != NULL)
	{
		fputs(head->id, path);
		fputs(head->art_name, path);
		fputs(head->album_name, path);
		fputs(head->track_num, path);
		fputs(head->track_name, path);
		fputs("\n", path);
		temp = head;
		head = head->next;
		free(temp);
	}
	free(head);
	fprintf(log_file, "\nYou choose save and exit.\nCome again soon!\n\n");
	printf("\nYou choose save and exit.\nCome again soon!\n\n");
	fclose(path);
	fclose(log_file);
	return;
}

void main()
{
	node* head, * current_song, * prev_song;
	FILE* log_file;
	int choise = 0;
	log_file = fopen("log_file.txt","w");//first init of log file
	if (!log_file)
		printf("Log file File not found!\n");
	printf("WELCOME TO DANIEL'S MP3!!\nBefore we start here is the song that we already have :\n\n");
	fprintf(log_file, "WELCOME TO DANIEL'S MP3!!\nBefore we start here is the song that we already have :\n\n");
	fclose(log_file);
	importData("playlist.txt");
	head = sorting_file_to_list("playlist.txt");
	current_song = head->next;
	prev_song = head->next;
	while (choise != 8)
	{
		log_file = fopen("log_file.txt", "a");
		printf("\nPlease choose an activity for the MP3 player.\n(1) - Play Current song\n(2) - Play next song\n(3) - Play prev song\n(4) - Search for song\n(5) - Delete song\n(6) - Insert Song\n(7) - Sorting the playlist\n(8) - Save and exit\n");
		fprintf(log_file,"\nPlease choose an activity for the MP3 player.\n(1) - Play Current song\n(2) - Play next song\n(3) - Play prev song\n(4) - Search for song\n(5) - Delete song\n(6) - Insert Song\n(7) - Sorting the playlist\n(8) - Save and exit\n");
		fclose(log_file);
		scanf("%d", &choise);
		switch (choise)
		{
		case 1:
			log_file = fopen("log_file.txt", "a");
			printf("Current song is :%-5s%-30s%-30s%-10s%-30s\n", current_song->id, current_song->art_name, current_song->album_name, current_song->track_num, current_song->track_name);
			fprintf(log_file, "%d\n", choise);
			fprintf(log_file,"Current song is :%-5s%-30s%-30s%-10s%-30s\n", current_song->id, current_song->art_name, current_song->album_name, current_song->track_num, current_song->track_name);
			fclose(log_file);
			break;
		case 2:
			log_file = fopen("log_file.txt", "a");
			current_song = NextSong(head, current_song);
			printf("Next song is :%-5s%-30s%-30s%-10s%-30s\n", current_song->id, current_song->art_name, current_song->album_name, current_song->track_num, current_song->track_name);
			fprintf(log_file, "%d\n", choise);
			fprintf(log_file,"Next song is :%-5s%-30s%-30s%-10s%-30s\n", current_song->id, current_song->art_name, current_song->album_name, current_song->track_num, current_song->track_name);
			fclose(log_file);
			break;
		case 3:
			log_file = fopen("log_file.txt", "a");
			current_song = PrevSong(head, current_song);
			printf("Previous song is :%-5s%-30s%-30s%-10s%-30s\n", current_song->id, current_song->art_name, current_song->album_name, current_song->track_num, current_song->track_name);
			fprintf(log_file, "%d\n", choise);
			fprintf(log_file,"Previous song is :%-5s%-30s%-30s%-10s%-30s\n", current_song->id, current_song->art_name, current_song->album_name, current_song->track_num, current_song->track_name);
			fclose(log_file);
			break;
		case 4:
			log_file = fopen("log_file.txt", "a");
			fprintf(log_file, "%d\n", choise);
			fclose(log_file);
			SearchSong(head);
			break;
		case 5:
			log_file = fopen("log_file.txt", "a");
			fprintf(log_file, "%d\n", choise);
			fclose(log_file);
			DeleteSong(head);
			break;
		case 6:
			log_file = fopen("log_file.txt", "a");
			fprintf(log_file, "%d\n", choise);
			fclose(log_file);
			InsertSong(head);
			break;
		case 7:
			log_file = fopen("log_file.txt", "a");
			fprintf(log_file, "%d\n", choise);
			fclose(log_file);
			SortBy(head);
			break;
		case 8:
			log_file = fopen("log_file.txt", "a");
			fprintf(log_file, "%d\n", choise);
			fclose(log_file);
			SaveAndExit("playlist.txt", head);
			break;
		}
	}
}