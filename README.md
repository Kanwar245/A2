# Buxfer based simple application

## Introduction

Buxfer is a service that lets groups of people track shared expenses. For example, roommates might want to track shared expenses such as rent, utilities, and groceries, or colleagues might want to keep track of shared lunch bills. For each expense, Buxfer records the person who paid the expense and the expense cost. It allows group members to examine the amount that individuals have paid, look at the history of all transactions, or determine the group member that is currently owing the most. Buxfer started as a small C program written by three graduate students at Carnegie Mellon University for their personal use and has since grown into a full-blown company! This project implements a basic version of Buxfer.


## Data structures

A Buxfer application can manage multiple groups, and for each group must keep track of the group's members and the group's transactions (expenses paid by users). This means that we need three different data structures: one to keep track of the groups, one to keep track of the users of a group, and one for a group's transactions. Since we don't know the number of groups that will be created, the users that will join a group, or the number of transactions that will be posted, we will use linked lists to store groups, users and transactions. The picture below shows what these data structures look like for an example with two different groups, one with two users and three transactions, and the other with one user and two transactions. Three linked lists, one for groups, one for users, and one for transactions. Group nodes point to user and transaction nodes.

![alt text] (https://github.com/Kanwar245/SimpleBuxfer/blob/master/list.jpg)

A node in the linked list of groups (the list arranged vertically on the left in the picture above) stores the name of the group, a pointer to the first node of the linked list that stores the users of this group, a pointer to the first node of the linked list that stores all the transactions posted for this group, and a pointer to the next group list node.

A user list node keeps track of the relevant information for this user, i.e. his/her name, balance (the total amount of expenses he/she has paid so far) and a pointer to the next element of the list of users.

A transaction (xct) list node keeps track of all the information about a transaction, i.e. the name of the user who paid for it, the amount, and a pointer to the next transaction list node.

The user lists are sorted by the user's balances, with the user with the lowest balance first in the list and the user with the highest balance last in the list. (The order among users who have the same balance does not matter.) This makes it easier to find the user who has paid the least so far. The transaction lists are sorted by the time when a transaction was added, with the most recent transaction first in the list. This makes it easy to output a sorted history of the transactions in a group.

## Operations supported by Buxfer

A Buxfer application supports the following user commands.

    * add_group group_name: Register a new group with name group_name.
    * list_groups: List the names of all groups that are currently registered.
    * add_user group_name user_name: Register a new user with name user_name as a member of group group_name.
    * list_users group_name: List the names of all users of group group_name together with their current balances (i.e. how much each has paid so far), sorted by their balances (lowest payer first).
    * user_balance group_name user_name: Return the balance of user user_name in group group_name
    * remove_user group_name user_name: Remove user user_name from group group_name
    * under_paid group_name: Output the name of the user in group group_name who has paid the least.
    * add_xct group_name user_name amount: Add a new transaction for group group_name. The transaction is paid by user_name and the amount is amount.
    * recent_xct group_name num_xct: List the num_xct most recent transactions for group group_name.
    * quit: Shut down buxfer 

The buxfer executable can be started from the commandline with either zero or one argument as follows:

 ./buxfer [filename] 

If buxfer is run without any arguments, it starts in interactive mode, i.e. it will display a prompt and wait for the above commands at the command line. If run with one argument, it expects this argument to be the name of a file that contains one buxfer command per line (the commands are those from the list above) and will execute those commands from the file.

## Code

The repository contains a Makefile and three files: buxfer.c, lists.h, and lists.c that provide the implementation of a skeleton of the buxfer data structures. buxfer.c provides the code to read and parse the user commands described above, and then calls the corresponding C function that implements the functionality of a user command. When run without command line arguments, buxfer will wait for commands from the console (standard input); when run with a filename as a command line argument, it will read commands from the specified input file. Function prototypes for all functions implementing the various user commands are provided in lists.h.

You can compile the code using the provided Makefile. You simply need to run the command make (without any arguments) inside the toplevel directory containing all the required files. This will create an executable called buxfer.

The sample input file batch_commands.txt contains a series of buxfer user commands that buxfer will execute in batch mode if run with the name of the file as a command line argument.
