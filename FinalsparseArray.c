// C program for Sparse Matrix Representation 
// using Linked Lists 
#include<stdio.h> 
#include<stdlib.h> 

// Node to represent sparse matrix 
struct Node 
{ 
    int value; 
    int rowPosition; 
    int columnPosition; 
    struct Node *right;
    struct Node *left; 
    struct Node *down; 
    struct Node *up;  
};



//function to create the "grid coordinates", these are nodes as well
void gridCreation (struct Node** start, int numberOfRows, int numberOfColumns)
{
    struct Node *temp, *newNode;
	int i;
	
	//printf("just entered grid creation function! \n");
	
    temp = (struct Node *) malloc (sizeof(struct Node)); 
    *start = temp;  
    for ( i=0 ; i< numberOfColumns ; i++) {
    	//printf("just entered %d loop to create column header! \n", i);
        newNode =  (struct Node *) malloc (sizeof(struct Node));
        //printf("in %d loop to create column header! allocated memory for a node \n", i);
        newNode->columnPosition = i;
        newNode->rowPosition = -1;
        //printf("in %d loop to create column header! assigned the column \n", i);
        newNode->left = temp;
        //printf("in %d loop to create column header! connected new node to point left to the previous \n", i);
        temp->right = newNode; 
        newNode->right = NULL; //initialize to null
        newNode->up = NULL; //initialize to null
        newNode->down = NULL; //initialize to null
        temp = newNode;
        //printf("in %d loop to create column header! reached the end of the loop \n", i);
    }

    temp = *start;
    for (i=0 ; i< numberOfRows ; i++) {
    	//printf("just entered %d loop to create column row! \n", i);
        newNode =  (struct Node *) malloc (sizeof(struct Node));
        newNode->rowPosition = i;
        newNode->columnPosition = -1;
        newNode->up = temp;
        temp->down = newNode;
        newNode->right = NULL; //initialize to null
        newNode->left = NULL; //initialize to null
        newNode->down = NULL; //initialize to null
        temp = newNode;
    }
    temp= *start;
    temp->columnPosition=-1;
    temp->rowPosition=-1;

}

// Function to insert new node 
void insertNode(struct Node** start, int data, int rowIndex, int columnIndex ) 
{ 
    struct Node *temp, *newNode;
    temp = *start;
	
	//printf("got in insert node\n");
	
	//connects the node left and right in the correct position
    //go down until reaching the correct row
    temp = temp->down; 
    while (temp->rowPosition < rowIndex){
        temp = temp->down;
    }
    //printf("got under the first while\n");
    //go right without passing the correct column
    while ((temp->right != NULL) && (temp->right->columnPosition < columnIndex)){//first we check if the next node to the right is not NULL cause that is necessary for the second condition of the while to be evaluated
        temp = temp->right;
    }
    //printf("got under the second while\n");

    if (temp->right == NULL){  //if at the end of the list create a new node at the end
        newNode = (struct Node*) malloc (sizeof(struct Node));
        newNode->value = data;
        newNode->rowPosition = rowIndex;
        newNode->columnPosition = columnIndex;
        newNode->left = temp;
        newNode->right = NULL; //initialize to null
        temp->right = newNode;
        printf("added the element %d\n", newNode->value);
    }
    else if (temp->right->columnPosition > columnIndex){// insert the new node in the left-right list
        newNode = (struct Node*) malloc (sizeof(struct Node));
        newNode->value = data;
        newNode->rowPosition = rowIndex;
        newNode->columnPosition = columnIndex;
        newNode->left = temp;
        newNode->right = temp->right;
        temp->right->left = newNode;
        temp->right = newNode;
        printf("inserted the element %d\n", newNode->value);
    }
    else /*(temp->right->columnPosition == columnIndex)*/ { //if a node exists in the correct position, replace its value-data
        temp = temp->right;
        temp->value = data;
        printf("replaced the element %d\n", temp->value);
    }

    temp = *start;
    temp = temp->right; //go right once cause start does not have position and row values
    
	//connects the node up and down in the correct position
	//go right until reaching the correct column
    while (temp->columnPosition < columnIndex){
        temp = temp->right;
    }
    //go down without passing the correct row
    while (temp->down != NULL && temp->down->rowPosition < rowIndex){//first we check if the next node downwards is not NULL cause that is necessary for the second condition of the while to be evaluated
        temp = temp->down;
    }

    if (temp->down == NULL){ 
        newNode->up = temp;
        newNode->down = NULL; //initialize to null
        temp->down = newNode;
    }
    else if (temp->down->rowPosition > rowIndex){
        newNode->up = temp;
        newNode->down = temp->down;
        temp->down->up = newNode;
        temp->down = newNode;
    }
    else /*(temp->right->columnPosition == columnIndex)*/ { 
        //no need to be here!
    }
}


void printArray(struct Node** start){
    struct Node *temp, *startOfRow;
    temp = *start;

    //parse the rows
    while(temp->down !=NULL){ 
        temp = temp->down;
        startOfRow = temp;
        while(temp->right != NULL ) {
            temp = temp->right;
            printf("Row : ");
            printf(" %d ", temp->rowPosition);
            printf("\n"); 
            printf("Column : ");
            printf(" %d ", temp->columnPosition);
            printf("\n");
            printf("Value : ");
            printf(" %d ", temp->value);
            printf("\n");
            printf("---------------------");
            printf("\n");
        }
        temp = startOfRow;

    }
}

void printArray2(struct Node** start, int numberOfRows, int numberOfColumns){
    struct Node *temp, *startOfRow;
    temp = *start;
    int distanceOfNodes=0, i;

    //parse the rows
    while(temp->down !=NULL){ 
        temp = temp->down;
        startOfRow = temp;
        while(temp->right != NULL ) {
            temp = temp->right;
            
            //check the zeros from the beginning of the line untill the firts element or bettween two elements
            distanceOfNodes = temp->columnPosition - temp->left->columnPosition -1;
            while (distanceOfNodes >0){
                    printf("   0 ");
                    distanceOfNodes--;
                }
            printf(" %3d ", temp->value);
            }
        
        //complete the line with zeros 
            distanceOfNodes = numberOfColumns - temp->columnPosition -1;
            while (distanceOfNodes >0){
                printf("   0 ");
                distanceOfNodes--;
            }
            printf("\n");
            temp = startOfRow;
    }
    distanceOfNodes = numberOfRows - temp->rowPosition -1;
    while (distanceOfNodes > 0){
        for (i =0; i<numberOfColumns ; i++){
            printf("   0 ");
        }
        printf("\n");
        distanceOfNodes--;
    }
}

void deleteElement(struct Node **start, int rowIndex, int columnIndex){
    struct Node *temp, *startOfRow;
    temp = *start;

    
    //go down until reaching the correct row
    while ((temp->down!=NULL)&&(temp->down->rowPosition <= rowIndex)){
        temp = temp->down;
    }
    //go right until reaching the correct column
    while ((temp->right != NULL) && (temp->right->columnPosition <= columnIndex)){//first we check if the next node to the right is not NULL cause that is necessary for the second condition of the while to be evaluated
        temp = temp->right;
    }

    if ((temp->rowPosition == rowIndex)&&(temp->columnPosition==columnIndex)){  //if at the end of the list create a new node at the end
       
        temp->left->right = temp->right;
        temp->up->down = temp->down;
       if (temp->down !=NULL){
        temp->down->up = temp->up;
       } 
       if( temp->right!=NULL){
        temp->right->left = temp->left;   
       }
        
        printf("deleted the element with value %d , in the row %d and column %d \n", temp->value, temp->rowPosition, temp->columnPosition);
        free(temp);
    }

    else{
        printf("No element in the row %d and column %d \n",  rowIndex, columnIndex );
    }
}

void printgrid(struct Node** start){
	struct Node *temp;
	temp=*start;
	
	printf("column position: ");
	
	while (temp->right != NULL){
		printf(" %d ", temp->columnPosition);
        		temp = temp->right;

	}
	printf("\n");
    
}

void menuToCreateTheGrid(struct Node** start){
    int rows;
    int columns;
    printf("Give the rows of the array");
    scanf("%d",&rows);  
    printf("Give the columns of the array");
    scanf("%d",&columns); 
	gridCreation(&start, rows, columns);
}

int main (){
	
    // menuToCreateTheGrid();

	int j, i;
	int matrix[4][5] = {
	{0,0,3,0,4},
	{0,0,5,7,0},
	{0,0,0,0,0},
	{0,2,6,0,0}
	};
	
	struct Node *start = NULL;

    //create the grid
    //The array has to be always square
	// int gridRows;
    // int gridColumns;
    // printf("\nGive the rows of the square array\n");
    // scanf("%d",&gridRows);  
    // printf("Give the columns of the array\n");
    // scanf("%d",&gridColumns); 
	// gridCreation(&start, gridRows, gridRows);
    menuToCreateTheGrid(&start);
        
    char select='k';
	int value;
    int row;
    int column;
    // printf("To select einai %c prin to loop\n", select);
    do{
        printf("\n--------------------------------------------------");
        printf("\nYour options are (i)nsert, (p)rint, (d)elete, (q)uit \n");
        printf("--------------------------------------------------\n");
        scanf(" %c", &select);
        // printf("To select einai %c prin ti switch\n", select);
        switch(select) {
            //Insert an element
            case 'i' :
                printf("\nGive the value you want you insert in the array \n");
                scanf("%d", &value);
                printf("\nGive the row \n");
                scanf("%d",&row);  
                printf("\nGive the column \n");
                scanf("%d",&column); 
                // if ((row<gridRows)&&(column<gridColumns)){
                if ((row<gridRows)&&(column<gridRows)){
                    insertNode(&start, value, row, column);
                    printf("Insert the symetric element\n");
                    //This is the symetric value
                    insertNode(&start, value, column,row);
                }
                else {
                    printf("\n*** You can not exceed the grid !!! ***\n");
                }
                break;
	
            //print the array
            case 'p' :
                // printArray2(&start,gridRows,gridColumns);
                printArray2(&start,gridRows,gridRows);
                // printf("To select einai %c mesa sti switch\n", select);
                break; 

            //delete an element
            case 'd' :
                printf("\nGive the row of the element you want to delete \n");
                scanf("\n%d",&row);  
                printf("\nGive the column of the element you want to delete\n");
                scanf("\n%d",&column); 
                // if ((row<gridRows)&&(column<gridColumns)){
                if ((row<gridRows)&&(column<gridRows)){
                    deleteElement(&start,row,column);
                    printf("Delete the symetric element\n");
                    //Delete the symetric value
                    deleteElement(&start, column, row);
                }
                else {
                    printf("\n*** You can not exceed the grid !!! ***\n");
                }
                break; 
             case 'q' :
                printf("You want to quit\n");
                break; 

            default : 
                printf("\n*** Wrong input ***\n");
                break; 
            }
    }while(select !='q');

    printf("\nGoodBye !!!");



	
	// printgrid(&start);
	
    //doesnt; check if the inserted elements exceeds the grid 
	// insertNode(&start, 2, 0, 0);
	// insertNode(&start, 1, 0, 2);
	// insertNode(&start, 2, 2, 0);
    // insertNode(&start, 7, 2, 1);
    // insertNode(&start, 9, 3, 2);
    // printArray2(&start,4,3 );



// printArray(&start);
// printArray2(&start,5,4 );
// deleteElement(&start,2,1);
// printArray2(&start,5,4 );
// deleteElement(&start,2,1);
// printArray2(&start,5,4 );
// deleteElement(&start,0,2);
// printArray2(&start,5,4 );
// deleteElement(&start,2,0);
// printArray2(&start,5,4);
// deleteElement(&start,3,2);
// printArray2(&start,5,4);
// deleteElement(&start,0,0);
// printArray2(&start,5,4);


	
//	for (i = 0; i<4 ; i++)
//		//printf("in first loop num %d \n", i);
//		for ( j = 0; j<4 ; j++)
//			//printf("in second loop num %d \n", j);
//			if (matrix[i][j] != 0)
//				insertNode(&start, matrix[i][j], i, j);
				
	
	return 1;
}
