
#include<stdio.h>
#include<stdlib.h>
#include"NoteSynth.c"

typedef struct BST_Node_Struct
{

    double key;
    double freq;
    int bar;
    double index;

    struct BST_Node_Struct *left;
    struct BST_Node_Struct *right;
  
} BST_Node;

BST_Node *newBST_Node(double freq, int bar, double index)
{

	BST_Node *new_node = NULL;

	new_node = (BST_Node *)calloc(1, sizeof(BST_Node));

	new_node->freq = freq;
	new_node->bar = bar;
	new_node->index = index;
	new_node->key = (10.0*bar)+index;
	new_node->left = NULL;
	new_node->right = NULL;
        
    return new_node;
}

BST_Node *BST_insert(BST_Node *root, BST_Node *new_node)
{
	if(root == NULL){
		return new_node;
	}

	if(root->key > new_node->key){
		root->left = BST_insert(root->left, new_node);
	} else if(root->key < new_node->key) {
		root->right = BST_insert(root->right, new_node);
	} else {
		printf("Duplicate node requested (bar:index)=%d,%lf, it was ignored\n", new_node->bar, new_node->index);
		return root;
	}

	return root;
}

BST_Node *BST_search(BST_Node *root, int bar, double index)
{

	if(root == NULL){
		return NULL;
	}

	if(fabs(root->key - ((10.0*bar)+index)) < 0.0001 ){
		return root;
	}

	if(root->key > ((10.0*bar)+index)){
		return BST_search(root->left, bar, index);
		printf("1\n");
	} else {
		return BST_search(root->right, bar, index);
		printf("2\n");
	}
}

BST_Node *find_successor(BST_Node *right_child_node)
{

	BST_Node *temp = NULL;
	temp = right_child_node;

	while(temp->left != NULL){
		temp = temp->left;
	}

    return temp;
    
}

BST_Node *BST_delete(BST_Node *root, int bar, double index)
{
	BST_Node *temp = NULL;

	if(root == NULL){
		return root;
	}

	if(fabs(root->key - ((10.0*bar)+index)) < 0.0001){
		if(root->left == NULL && root->right == NULL){
			free(root);
			return NULL;
		} else if(root->left == NULL){
			temp = root->right;
			free(root);
			return temp;
		} else if(root->right == NULL){
			temp = root->left;
			free(root);
			return temp;
		} else {
			temp = find_successor(root->right);
			root->key = temp->key;
			root->bar = temp->bar;
			root->index = temp->index;
			root->freq = temp->freq;
			
			root->right = BST_delete(root->right, temp->bar, temp->index);
		}
	}

	if(root->key < ((10.0*bar)+index)){
		root->right = BST_delete(root->right, bar, index);
	} else {
		root->left = BST_delete(root->left, bar, index);
	}

	return root;
}

void BST_makePlayList(BST_Node *root)
{
	if(root == NULL){
		return;
	}

	BST_makePlayList(root->left);

	playlist_head = playlist_insert(playlist_head, root->freq, root->bar, root->index);

	BST_makePlayList(root->right);

}

void BST_inOrder(BST_Node *root, int depth)
{

	if(root == NULL){
		return;
	}

	BST_inOrder(root->left, depth+1);

	printf("Depth=%d, Bar:Index (%d:%f), F=%f Hz\n", depth, root->bar, root->index, root->freq);

	BST_inOrder(root->right, depth+1);

} 

void BST_preOrder(BST_Node *root, int depth)
{

	if(root == NULL){
		return;
	}

	printf("Depth=%d, Bar:Index (%d:%f), F=%f Hz\n", depth, root->bar, root->index, root->freq);

	BST_preOrder(root->left, depth+1);

	BST_preOrder(root->right, depth+1);

}

void BST_postOrder(BST_Node *root,int depth)
{

	if(root == NULL){
		return;
	}

	BST_postOrder(root->left, depth+1);

	BST_postOrder(root->right, depth+1);

	printf("Depth=%d, Bar:Index (%d:%f), F=%f Hz\n", depth, root->bar, root->index, root->freq);

} 

void delete_BST(BST_Node *root)
{

	if(root == NULL){
		return;
	}

	delete_BST(root->left);

	delete_BST(root->right);

	free(root);

}

void BST_shiftFreq(BST_Node *root, char note_src[5], char note_dst[5])
{

	double freq_finding;
    double freq_replacing;

    for(int i=0; i<100; i++){
        if(strcmp(note_names[i], note_src) == 0){
            freq_finding = note_freq[i];
        }

        if(strcmp(note_names[i], note_dst) == 0){
            freq_replacing = note_freq[i];
        }
    }

    if(root != NULL){
        if(root->left != NULL){
            BST_shiftFreq(root->left, note_src, note_dst);
        }

        if(fabs(root->freq - freq_finding) <0.0001){
            root->freq = freq_replacing;
        }

        if(root->right != NULL){
            BST_shiftFreq(root->right, note_src, note_dst);
        }
    }
}

typedef struct pointer_to_harmonized_notes
{
	BST_Node *harmonized_note;
	struct pointer_to_harmonized_notes *next; 
}list_node;

list_node *insert_to_tail(list_node *head, double freq, int bar, double index)
{	
	BST_Node *new_node = NULL;
	list_node *new_list_node = NULL;
	//list_node *temp = NULL;

	//creating a BST node
	new_node = newBST_Node(freq, bar, index);

	//creating list node
	new_list_node = (list_node *)calloc(1, sizeof(list_node));
	new_list_node->harmonized_note = new_node;
	new_list_node->next = NULL;

	if(head == NULL){
		return new_list_node;
	}

	new_list_node->next = head;
	return new_list_node;
}

int result_semitone(BST_Node *root, int semitones)
{
	int current_semitone;
	for(int i=0; i<100; i++){
		if(root->freq == note_freq[i]){
			current_semitone = i;
			break;
		}
	}

	int resulting_semitone = current_semitone + semitones;
	
	return resulting_semitone;
}

list_node *delete_list(list_node *head){
	list_node *p = NULL;
	list_node *q = NULL;

    while(p != NULL){
    	q = p->next;
    	free(p);
    	p = q;
    }    

    return NULL;
}

BST_Node *BST_insert_for_harmonize(BST_Node *root, BST_Node *new_node)
{
	if(root == NULL){
		return new_node;
	}

	if(root->key > new_node->key){
		root->left = BST_insert_for_harmonize(root->left, new_node);
	} else if(root->key < new_node->key) {
		root->right = BST_insert_for_harmonize(root->right, new_node);
	} else {
		new_node->index = new_node->index + 0.0001;
        root->left = BST_insert_for_harmonize(root->left, new_node);
	}

	return root;
}

BST_Node *BST_harmonize(BST_Node *root, int semitones, double time_shift)
{


	double resulting_index;
	int resulting_semitone;
	BST_Node *temp = NULL;
	BST_Node *predecessor;
	list_node *head = NULL;
	list_node *tr = NULL;

	temp = root;

	if(root != NULL){	
		while(temp != NULL){
			if(temp->left == NULL){
				resulting_semitone = result_semitone(temp, semitones);
				resulting_index = temp->index + time_shift;
				if(resulting_semitone >=0 && resulting_semitone<=99 && resulting_index>=0 && resulting_index<1){
					head = insert_to_tail(head, note_freq[resulting_semitone], temp->bar, resulting_index);
				}
				temp = temp->right;
			} else {
				predecessor = temp->left;
				while(predecessor->right != NULL && predecessor->right != temp){
					predecessor = predecessor->right;
				}

				if(predecessor->right == NULL){
					predecessor->right = temp;
					temp = temp->left;
				} else {
					predecessor->right = NULL;
					resulting_semitone = result_semitone(temp, semitones);
					resulting_index = temp->index + time_shift;
					if(resulting_semitone >=0 && resulting_semitone<=99 && resulting_index>=0 && resulting_index<1){
						head = insert_to_tail(head, note_freq[resulting_semitone], temp->bar, resulting_index);
					}
					temp = temp->right;
				}
			}
		}
	} else {
		return root;
	}

	tr = head;

	while(head != NULL){
		root = BST_insert_for_harmonize(root, head->harmonized_note);
		head = head->next;
	}

	tr = delete_list(tr);

	return root;
}
