static char mot_a_trouver[WORD_LEN+1];

void process_cmd_motus(client_t* client, char*param) {
  get_random_word(mot_a_trouver);
  char mot_masque[WORD_LEN+1];
  mot_masque[0] = mot_a_trouver[0];
  for(int i = 1; i < WORD_LEN; i++)
    mot_masque[i] = '-';
  mot_masque[WORD_LEN] = '\0';

  char buff_out[1024];
  sprintf(buff_out, "* Motus: new word: %s\n", mot_masque);
  send_message_all(buff_out);
}

static void get_random_word(char word[WORD_LEN+1]) {
  char* filename="wordlist.dat";
  FILE*f = fopen(filename, "r");
  if(! f) {
    fprintf(stderr, "Cannot open %s\n", filename);
    abort();
  }

  fseek(f, 0, SEEK_END);
  int pos = ftell(f);
  int nbwords= pos/(WORD_LEN+1);
  int index = rand() % nbwords;
  fseek(f, index * (WORD_LEN+1), SEEK_SET);
  fread(word, sizeof(char), WORD_LEN, f);
  fclose(f);
}


static int comparer_mots(char mot_propose[WORD_LEN+1], char mot_masque[WORD_LEN+1]) {
    int ncorrect = 0;
    for (int i = 0; i < WORD_LEN; i++) {
        /* make sure the proposed word is uppercase */
        mot_propose[i] = upper(mot_propose[i]);
        if (mot_propose[i] == mot_a_trouver[i]) {
            mot_maque[i] = toupper(mot_propose[i]);
            ncorrect++;
        }
        else {
            mot_maque[i] = '-';
            for (int j=0; j < WORD_LEN; j++) {
                if (mot_propose[i] == mot_a_trouver[j]) {
                    mot_maque[i] = lower(mot_propose[i]);
                    break;
                }
            }
        }
    }
    mot_masque[WORD_LEN] = '\0';
    return ncorrect;
}

void process_cmd_classement(client_t* client, char*param) {
    char buff_out[1024];
    sprintf(buff_out, "* Classement: \n");
    sprintf(buff_out, "* ----------: \n");
    send_message(buff_out, client);
    struct joueur *cur = leaderboard;
    int i=1;
    while (cur != NULL)
    {
        sprintf(buff_out, "* [%d] %s (%d points) \n", i, cur->client->name, cur->score);
        send_message(buff_out, client);
        cur = cur->next;
        i++;
    }
}


void add_points(client_t* client, int nb_points) {
    if (leaderboard  == NULL)
    {
        struct joueur* j = malloc(sizeof(struct joueur));
        j->client = client;
        j->score = nb_points;
        j->next-> = NULL;
        leaderboard = j;
        return;
    }
    struct client_node *previous_node = NULL;
    struct client_node *cur_node = leaderboard;
    struct client_node *next_node = leaderboard->next;
    while (next_node != NULL && cur_node->client->uid != client->uid)
    {
        previous_node = cur_node;
        cur_node = next_node;
        next_node = next_node->next;
    }

    struct joueur* j = NULL;

    if (next_node == NULL && cur_node->client->uid != client->uid)
    {
        struct joueur* tmp_j = malloc(sizeof(struct joueur));
        j->client = client;
        j->score = nb_points;
        j->next-> = NULL;
        j = tmp_j;
    }
    else {
        previous_node->next = next_node;
        cur_node->score += nb_points;
        j = cur_node;
    }
    free(previous_node);
    free(cur_node);
    free(next_node);

    // On insert j dans la liste
    struct client_node *previous_node = NULL;
    struct client_node *cur_node = leaderboard;
    while (cur_node != NULL && cur_node->score > j->score)
    {
        previous_node = cur_node;
        cur_node = cur_node->next;
    }
    if (previous_node == NULL)
    {
        j->next = cur_node;
        leaderboard = j;
    }
    previous_node->next = j;
    j->next = cur_node;
}