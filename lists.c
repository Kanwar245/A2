#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lists.h"

/* Add a group with name group_name to the group_list referred to by
* group_list_ptr. The groups are ordered by the time that the group was
* added to the list with new groups added to the end of the list.
*
* Returns 0 on success and -1 if a group with this name already exists.
*
* (I.e, allocate and initialize a Group struct, and insert it
* into the group_list. Note that the head of the group list might change
* which is why the first argument is a double pointer.)
*/
int add_group(Group **group_list_ptr, const char *group_name) {

    Group *new_group = malloc(sizeof(Group));
    /*if malloc failed, print error message and exit*/
    if (new_group == NULL) {
        perror("Malloc");
        exit(EXIT_FAILURE);
    }

    /*need to allocate space to store the group's name*/
    new_group->name = malloc(strlen(group_name) + 1);
    if (new_group->name == NULL) {
        perror("malloc");
        exit(EXIT_FAILURE);
    }
    /*initialize the new group*/
    strcpy(new_group->name, group_name);
    new_group->next = NULL;
    new_group->users = NULL;
    new_group->xcts = NULL;
 
    Group *current_group = *group_list_ptr;
    /*empty group case*/
    if (current_group == NULL) {
        *group_list_ptr = new_group;
        (*group_list_ptr)->next = NULL;
        return 0;
    }
    while (current_group->next != NULL) {
        if (strcmp(current_group->name, group_name) == 0) {
            return -1;
        }
        current_group = current_group->next;
    }
    /*case with 1 group or when all groups have been traversed*/
    if (strcmp(current_group->name, group_name) == 0) {
        return -1;
    }
    current_group->next = new_group;
    return 0;
}

/* Print to standard output the names of all groups in group_list, one name
*  per line. Output is in the same order as group_list.
*/
void list_groups(Group *group_list) {

    Group *current_group = group_list;
    /*if group_list is empty return*/
    if (current_group == NULL) {
        return;
    }
    /*otherwise traverse the group list till the last group*/
    while (current_group != NULL) {
        printf("Group's name: %s\n",current_group->name);
        current_group = current_group->next;
    }
}

/* Search the list of groups for a group with matching group_name
* If group_name is not found, return NULL, otherwise return a pointer to the 
* matching group list node.
*/
Group *find_group(Group *group_list, const char *group_name) {

    Group *current_group = group_list;
    /*if group_list is empty return NULL*/
    if (current_group == NULL) {
        return NULL;
    }
    /*while current group is not null*/
    while (current_group != NULL) {
        if (strcmp(current_group->name, group_name) == 0) {
            return current_group;
        }
        current_group = current_group->next;
    }
    /*no matching group found, so return NULL*/
    return NULL;
}

/* Add a new user with the specified user name to the specified group. Return zero
* on success and -1 if the group already has a user with that name.
* (allocate and initialize a User data structure and insert it into the
* appropriate group list)
*/
int add_user(Group *group, const char *user_name) {

    User *prev_user = find_prev_user(group, user_name);
	/*user with give name doesn't exist*/
	if (prev_user == NULL) {
		
		User *new_user = malloc(sizeof(User));
		if (new_user == NULL) {
			perror("malloc");
			exit(EXIT_FAILURE);
		}

		new_user->name = malloc(strlen(user_name) + 1);
		if (new_user->name == NULL) {
			perror("malloc");
			exit(EXIT_FAILURE);
		}

		/*initialization of new_user*/
		strcpy(new_user->name, user_name);
		new_user->balance = 0.0;
		new_user->next = NULL;

		/*empty user list*/
		if (group->users == NULL) {
			group->users = new_user;
			group->users->next = NULL;
			return 0;
		}
		
		/*if no user with same name exists or prev user is the the head, add new user to the front*/
		if (prev_user == NULL || prev_user == group->users) {
			new_user->next = group->users;
			group->users = new_user;
			return 0;
		}
	}
	/*otherwise user with same exists*/
	return -1;
}

/* Remove the user with matching user and group name and
* remove all her transactions from the transaction list. 
* Return 0 on success, and -1 if no matching user exists.
* Remember to free memory no longer needed.
* (Wait on implementing the removal of the user's transactions until you 
* get to Part III below, when you will implement transactions.)
*/
int remove_user(Group *group, const char *user_name) {

    User *current_user = group->users;
    User *temp = group->users;
    User *prev_user = find_prev_user(group, user_name);
    //printf("previous user: %s\n", prev_user->name);
    
	/*no matching user exists*/
	if (prev_user == NULL) {
		return -1;
	}
    /*find prev user returns the first user*/
    else if (prev_user == group->users) {
		/*only one user so remove it*/
		if (prev_user->next == NULL) {
			current_user = NULL;
			group->users = current_user;
			free(temp->name);
			free(temp);
			remove_xct(group, user_name);
			return 0;
		}
		/*case where find_prev_user returns the head of user list*/
		else if (strcmp(prev_user->next->name, user_name) != 0) {
			current_user = current_user->next;
			group->users = current_user;
			free(temp->name);
			free(temp);
			remove_xct(group, user_name);
			return 0;
		} 
		/*case where find_prev_user returns the second user*/
		else {
			temp = current_user->next;
			current_user->next = current_user->next->next;
			free(temp->name);
			free(temp);
			remove_xct(group, user_name);
			return 0;
		}
    }
	/*case where find_prev_user doesn't return first user*/
	else {
        while (current_user != prev_user) {
            current_user = current_user->next;
        }
        temp = current_user->next;
        current_user->next = current_user->next->next;
        free(temp->name);
        free(temp);
        remove_xct(group, user_name);
        return 0;
    }
}

/* Print to standard output the names of all the users in group, one
* per line, and in the order that users are stored in the list, namely 
* lowest payer first.
*/
void list_users(Group *group) {

    User *current_user = group->users;
    /*if group_list is empty return*/
    if (current_user == NULL) {
        return;
    }
    /*otherwise traverse the group list till the last group*/
    while (current_user != NULL) {
        printf("User: %s, Balance: %0.2f\n",current_user->name, current_user->balance);
        current_user = current_user->next;
    }

}

/* Print to standard output the balance of the specified user. Return 0
* on success, or -1 if the user with the given name is not in the group.
*/
int user_balance(Group *group, const char *user_name) {

    User *prev_user = find_prev_user(group, user_name);
	/*first user*/
    if (prev_user == group->users && strcmp(group->users->name, prev_user->next->name) != 0) {
        printf("Balance: %0.2f\n", group->users->balance);
        return 0;
	/*any other user*/
    } else if (prev_user != NULL) {
        printf("Balance: %0.2f\n", prev_user->next->balance);
        return 0;
    } else { /*user with given name is not in the group*/
        return -1;
    }
}

/* Print to standard output the name of the user who has paid the least 
* If there are several users with equal least amounts, all names are output. 
* Returns 0 on success, and -1 if the list of users is empty.
* (This should be easy, since your list is sorted by balance). 
*/
int under_paid(Group *group) {

    User *current_user = group->users;
    if (current_user != NULL) {
        /*first user has the least balance*/
        printf("User: %s\n", current_user->name);
        /*if there are more users with the same least balance*/
        while (current_user->balance == current_user->next->balance) {
            printf("User: %s\n", current_user->next->name);
            current_user = current_user->next;
        }
        return 0;
    }
    return -1;  /*empty user list*/
}

/* Return a pointer to the user prior to the one in group with user_name. If 
* the matching user is the first in the list (i.e. there is no prior user in 
* the list), return a pointer to the matching user itself. If no matching user 
* exists, return NULL. 
*
* The reason for returning the prior user is that returning the matching user 
* itself does not allow us to change the user that occurs before the
* matching user, and some of the functions you will implement require that
* we be able to do this.
*/
User *find_prev_user(Group *group, const char *user_name) {

    User *current_user = group->users;
    
    /*empty user list*/
    if (current_user == NULL) {
        return NULL;
    }
    /*non-empty list*/
    if (strcmp(current_user->name, user_name) == 0) {
        return current_user;
    }

    if (current_user->next == NULL) {
        if (strcmp(current_user->name, user_name) == 0) {
            return current_user;
        }
    } else {
        while (current_user->next != NULL) {
            if (strcmp(current_user->next->name, user_name) == 0) {
                return current_user;
            }
            current_user = current_user->next;
        }
    }
    return NULL;  /*all of the above fails*/
}

/* Add the transaction represented by user_name and amount to the appropriate 
* transaction list, and update the balances of the corresponding user and group. 
* Note that updating a user's balance might require the user to be moved to a
* different position in the list to keep the list in sorted order. Returns 0 on
* success, and -1 if the specified user does not exist.
*/
int add_xct(Group *group, const char *user_name, double amount) {

    User *temp, *next_user;
    User *prev_user = find_prev_user(group, user_name);

	if (prev_user == NULL) {
		return -1;
	}
	
	/*else prev user is not null*/
	
	/*allocate space for new transaction*/
	Xct *new_xct = malloc(sizeof(Xct));
	if (new_xct == NULL) {
		perror("malloc");
		exit(EXIT_FAILURE);
	}

	new_xct->name = malloc(strlen(user_name) + 1);
	if (new_xct->name == NULL) {
		perror("malloc");
		exit(EXIT_FAILURE);
	}

	/*initialize new_xct*/
	strcpy(new_xct->name, user_name);
	new_xct->amount = amount;
	new_xct->next = group->xcts;
	group->xcts = new_xct;

	/*if it's not the first user move to the next*/
	if (strcmp(prev_user->name, user_name) != 0) {
		prev_user = prev_user->next;
	}
	/*update user's balance*/
	prev_user->balance += amount;
	
	/*move the user to the right place in ascending order of user's balance*/
	while (prev_user->next != NULL && prev_user->balance >= prev_user->next->balance) {
		temp = find_prev_user(group, prev_user->name);
		next_user = prev_user->next;
		
		/*if the first user, update the balance, and make a new head*/
		if (strcmp(temp->name, user_name) == 0) {
			group->users = next_user;
		}
		else {
			temp->next = next_user;
		}
		/*insert at the right place*/
		prev_user->next = next_user->next;
		next_user->next = prev_user;
	}
	return 0;
}

/* Print to standard output the num_xct most recent transactions for the 
* specified group (or fewer transactions if there are less than num_xct 
* transactions posted for this group). The output should have one line per 
* transaction that prints the name and the amount of the transaction. If 
* there are no transactions, this function will print nothing.
*/
void recent_xct(Group *group, long num_xct) {

    Xct *current_xct = group->xcts;
    long accumulator = 0;
    if (current_xct == NULL || num_xct == 0) {
        return;
    }
    while (current_xct != NULL && accumulator != num_xct) {
        printf("Name: %s, Amount: %0.2f\n", current_xct->name, current_xct->amount);
        current_xct = current_xct->next;
        accumulator += 1;
    }
}

/* Remove all transactions that belong to the user_name from the group's 
* transaction list. This helper function should be called by remove_user. 
* If there are no transactions for this user, the function should do nothing.
* Remember to free memory no longer needed.
*/
void remove_xct(Group *group, const char *user_name) {

    Xct *current_xct = group->xcts;
	if (current_xct == NULL) {
        return;
    }
	Xct *next_xct = current_xct->next;
    Xct *temp;
	/*check for the first transaction*/
	while (strcmp(current_xct->name, user_name) == 0) {
		temp = current_xct;
		free(temp->name);
		free(temp);
		/*make new head*/
		group->xcts = next_xct;
		
		if (next_xct == NULL)
			return;
		/*otherwise move to the next xct*/
		current_xct = next_xct;
		next_xct = next_xct->next;
	}
	/*remaining transactions*/
	while (next_xct != NULL) {
		/*if name matches, free the matching xct*/
		if (strcmp(next_xct->name, user_name) == 0) {
			temp = next_xct;
			current_xct->next = next_xct->next;
			next_xct = next_xct->next;
			free(temp->name);
			free(temp);
		}
		/*otherwise move to the next xct*/
		else {
			current_xct = next_xct;
			next_xct = next_xct->next;
		}
	}
}