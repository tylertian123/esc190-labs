#include "lab4.h"

Player* create_player(char name[], char id[]){
	/**
		Creates and returns a pointer to a newly
		created dynamically allocated Person struct,
		with its <name> and <id> fields initialized
		to the values given by the respectively named 
		parameters.
		
		Hint: strcpy(...) from <string.h> may be helpful.
	*/
	Player *p = (Player*) malloc(sizeof(Player));
	strcpy(p->name, name);
	strcpy(p->id, id);
	return p;
}

PlayerRecord* create_leaf_record(Player* p){
	/**
		Creates and returns a pointer to a newly created
		dynamically allocated PlayerRecord struct. 
		
		The record created is for the starting bracket
		of a tournament, for player <p>, where <p> is
		a dynamically allocated Player struct as returned
		by create_player(...).
		
		Initializes the new PlayerRecord to have 0 wins 
		and 0 losses; the left_child, right_child, and 
		parent are set to NULL.
		
		The player field should be set to <p>.
	*/
	PlayerRecord *record = (PlayerRecord *) malloc(sizeof(PlayerRecord));
	record->left_child = record->right_child = record->parent = NULL;
	record->player = p;
	record->game_records[WINS] = record->game_records[LOSSES] = 0;
	return record;
}

PlayerRecord* add_match(
	PlayerRecord* p1, PlayerRecord* p2, 
	int p1_wins, int p2_wins
){
	/**
	Creates and returns a pointer to a newly created
	dynamically allocated PlayerRecord struct.
	
	Joins the tournament trees given by <p1> and <p2>, where
	<p1> and <p2> are dynamically allocated PlayerRecord structs
	as returned by create_leaf_record(...).

	Updates <p1->parent> and <p2->parent> to be the newly
	created PlayerRecord. Updates the left_child and right_child 
	of the newly created PlayerRecord to reflect the 
	loser and winner of the match, respectively. Fills the new
	PlayerRecord's game_records field.
	
	<p1->parent> and <p2->parent> are guaranteed to be NULL.
	*/
	PlayerRecord *winner = p1_wins > p2_wins ? p1 : p2;
	PlayerRecord *loser = p1_wins < p2_wins ? p1 : p2;
	PlayerRecord *record = create_leaf_record(winner->player);
	
	p1->parent = p2->parent = record;
	record->left_child = loser;
	record->right_child = winner;
	record->game_records[WINS] = p1_wins > p2_wins ? p1_wins : p2_wins;
	record->game_records[LOSSES] = p1_wins < p2_wins ? p1_wins : p2_wins;
	return record;
}

int get_player_rank(char player_id[], PlayerRecord* root){
	/**
	Get the rank of the player identified by <player_id>.
	<root> refers to the root of a completed tournament.
	
	You may assume that each player has a unique id.
	*/
	if (!root) {
		return 0;
	}
	if (!strcmp(root->player->id, player_id)) {
		return 1;
	}
	int r = get_player_rank(player_id, root->left_child);
	if (r) {
		return r + 1;
	}
	r = get_player_rank(player_id, root->right_child);
	return r ? r + 1 : r;
}

/**

	========== PROVIDED FUNCTIONS BELOW ==========
	DO NOT MODIFY.
*/

void clear_tournament_records_helper(
	PlayerRecord* curr, PlayerRecord*** records, 
	int* num_records, int* max_records
){
	/**
		Performs a pre-order traversal to retrieve all
		the player records and stores them in <records>,
		an array of pointer to PlayerRecord.
		
		Updates the dynamic array when needed by doubling the 
		size. If the dynamic array is resized, <*max_records> 
		is updated.
		
		<*num_records> is updated to reflect the total number 
		of records.
	*/
		
	if ((*max_records) == (*num_records)){
		// resize
		(*records) = realloc(*records, sizeof(PlayerRecord*) * (*max_records) * 2);
		(*max_records) *= 2;
	} 
	
	(*records)[*num_records] = curr;
	(*num_records)++;

	if (curr->left_child != NULL){
		clear_tournament_records_helper(
			curr->left_child, records, 
			num_records, max_records
		);
	}
	
	if (curr->right_child != NULL){
		clear_tournament_records_helper(
			curr->right_child, records, 
			num_records, max_records
		);
	}
}

void clear_tournament_records(PlayerRecord** root){
	/**
		Frees all memory associated with the tournament,
		EXCLUDING the Player(s)
	*/
	int num_records = 0;
	int max_records = 10;
	
	PlayerRecord** records = (PlayerRecord**)malloc(sizeof(PlayerRecord*) * max_records);
	if ((*root) != NULL){
		clear_tournament_records_helper(*root, &records, &num_records, &max_records);
	}
	
	for (int i=0; i<num_records; i++){
		free(records[i]);
	}
	*root = NULL;
	free(records);
}

void print_tournament_records(PlayerRecord* curr, PlayerRecord* root){
	/**
		Prints out the tournament records as a post-order traversal.
	*/
	if(curr->left_child != NULL){
		print_tournament_records(curr->left_child, root); 
		print_tournament_records(curr->right_child, root);
		
		fprintf(stdout, "%s VS %s\nRESULT: ", curr->left_child->player->id, curr->right_child->player->id);
	}
	
	fprintf(stdout, "%s with Wins: %d Losses: %d\n", 
		curr->player->id, 
		curr->game_records[WINS],
		curr->game_records[LOSSES]
	);
	
}

int player_in_list(Player* p, Player** player_list, int count_el){
	/**
		Returns 1 if <p> is found in <player_list>, returns 0 
		otherwise.
	*/
	for (int i=0; i<count_el; i++){
		if (p->id == (player_list[i])->id) {
			return 1;
		} 
	} return 0;
}

void collect_at_rank(
	PlayerRecord* curr,
	PlayerRecord* root, 
	Player** players_at_curr_rank, 
	int rank,
	int* count_el, 
	int* max_el
){	
	/**
		Collects all players with rank <rank> and stores them in <players_at_curr_rank>,
		updating <*count_el> and <*max_el> as needed.
	*/
	int tmp_rank = get_player_rank(curr->player->id, root);
	if (tmp_rank == rank && !player_in_list(curr->player, players_at_curr_rank, *count_el)){
		if ((*count_el) == (*max_el)){
			// resize
			players_at_curr_rank = realloc(players_at_curr_rank, sizeof(Player*) * (*max_el) * 2);
			(*max_el) *= 2;
		}
		players_at_curr_rank[*count_el] = curr->player;
		(*count_el)++;
	}
	if (curr->left_child != NULL){
		collect_at_rank(curr->left_child, root, players_at_curr_rank, rank, count_el, max_el);
	}
	if (curr->right_child != NULL){
		collect_at_rank(curr->right_child, root, players_at_curr_rank, rank, count_el, max_el);
	}
}

void print_with_rank(PlayerRecord* root){
	/**
	Prints a record of all players and their rank once the tournament
	has concluded. <root> refers to a completed tournament tree.
	
	Warning: very inefficient, do not use on large trees
	*/
	int count_el = 0;
	int max_el = 10;
	int curr_rank = 1;
	Player** players_at_curr_rank = (Player**)malloc(sizeof(Player*) * max_el);
	
	do {
		count_el = 0;
		collect_at_rank(root, root, players_at_curr_rank, curr_rank, &count_el, &max_el); 
		for (int i=0; i<count_el; i++){
			fprintf(stdout, "Rank %d: %s\n", curr_rank, (players_at_curr_rank[i])->id);
		}
		curr_rank++;
	} while (count_el != 0);
	
	free(players_at_curr_rank);
}


typedef struct QueueNode {
	PlayerRecord *record;
	int rank;
	struct QueueNode *next;
} QueueNode;

// Runs in O(n) best and worst case, where n is the number of players
void print_with_rank_fast(PlayerRecord *root) {
	// BFS with queue
	QueueNode *qhead = (QueueNode *) malloc(sizeof(QueueNode));
	QueueNode *qtail = qhead;
	qhead->record = root;
	qhead->rank = 1;
	qhead->next = NULL;

	while (qhead) {
		// Only print if root or losing match to avoid duplicates
		if (!qhead->record->parent || qhead->record->player != qhead->record->parent->player) {
			printf("Rank %d: %s\n", qhead->rank, qhead->record->player->id);
		}
		// Enqueue children
		if (qhead->record->left_child) {
			QueueNode *n = (QueueNode *) malloc(sizeof(QueueNode));
			n->record = qhead->record->left_child;
			n->rank = qhead->rank + 1;
			n->next = NULL;
			qtail->next = n;
			qtail = n;
		}
		if (qhead->record->right_child) {
			QueueNode *n = (QueueNode *) malloc(sizeof(QueueNode));
			n->record = qhead->record->right_child;
			n->rank = qhead->rank + 1;
			n->next = NULL;
			qtail->next = n;
			qtail = n;
		}
		// Dequeue head
		QueueNode *temp = qhead;
		qhead = qhead->next;
		free(temp);
	}
}
