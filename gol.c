#include <ncurses.h>
#include <math.h>
#include <time.h>

#define MAX_ROW 90
#define MAX_COL 25

//VARIABILI GLOBALI
int row = MAX_ROW+2;
int col = MAX_COL+2;

int bg_color=0;
int cl_color=1;

char viva = '@';
char morta = ' ';

int delay_ms = 120; //millisecondi

int grid[MAX_COL+2][MAX_ROW+2];
int new_grid[MAX_COL+2][MAX_ROW+2];

//FUNZIONI PROGRAMMA
void initColorPair(); //Inizializza le coppie colori
void wipe_grid(); //Ripulisce la griglia
void make_glider(int x, int y); //Disegna un glider a partire da x,y
void copy_grid(); //Copia la new_grid in grid
void fill_new_grid(); //Riempie le colonne laterali, righe esterne e gli angoli
void make_new_grid(); //Stabilisce la new_grid a partire da grid
void make_random_grid(); //Genera una griglia casuale di partenza
int check_life(int x, int y); //Applica le regola del gioco e restituisce lo stato della cellula x,y al passo successivo
void draw_grid(); //Discela grid, applicando i colori
void change_cell_color(); //Cambia il colore del background
void change_bg_color(); //Cambia il colore delle cellule

WINDOW *create_newwin(int height, int width, int starty, int startx);
void destroy_win(WINDOW *local_win);

int main(){

	//Box per le info
	WINDOW *info=NULL;

	//Inizializza la libreria ncurses
	initscr();

	//Buffering di linea disabilitato, passo al programma tutto
	raw();

	//Disasbilita l'echo dei tasti premuti
	noecho();

	//Disabilita il bloccaggio del programma per getch()
	nodelay(stdscr, TRUE);

	//Abilita i colori
	start_color();

	//Inizializza le coppie colore
	initColorPair();

	//Ripulisce la griglia
	wipe_grid();

	//Crea una griglia casuale
	make_random_grid();

	//Crea un gilder nella posizione specificata
//	make_glider(10,10);

	//Variabile per tenere il carattere tastiera
	int c;
	//Variabile di uscita ciclo
	int end=FALSE;
	//Variabile per visualizzare il box info
	int info_visible=FALSE;
	//Numero di cicli
	int ciclo=0;

	while(!end){
		//Operazioni principali
		//completare le colonne laterali,righe esterne e gli angoli
		fill_new_grid();
		//creare la nuova griglia controllando quella vecchia
		make_new_grid();
		//trasferire la nuova griglia a quella vecchia
		copy_grid();
		//stampare la griglia
		draw_grid();

		//Piccola interazione con l'utente
		c=getch();

		switch(c){

			case ' ':
				make_random_grid();
			break;

			case 'i':
				info_visible=TRUE; //Visualizza il box info
			break;

			case 'u':
				info_visible=FALSE; //Cancella il box info
			break;

			case 'q':
				end=TRUE; //Esce dal ciclo
			break;

			case 'c':
				change_cell_color(); //Cambia il colore delle cellule
			break;

			case 'b':
				change_bg_color(); //Cambia il colore dello sfondo
			break;

			case 'p':
				nodelay(stdscr, FALSE); //Blocca l'eseguzione del programma all'istruzione getch()
			break;

			case 'o':
				nodelay(stdscr, TRUE); //Non blocca l'eseguzione del programma su getch()
			break;

			case 'e':
				delay_ms=delay_ms+100; //Aumenta il delay di attesa alla fine di un ciclo
			break;

			case 'w':
				delay_ms=delay_ms-100; //Diminuisce il tempo di attesa alla fine del ciclo
			break;
		}

		//Controllo e stampa della finestra info
		if(info_visible){
			if(info==NULL)
				info=create_newwin(4,25,0,MAX_ROW+2);

			mvwprintw(info,1,1,"Ciclo n° %i",ciclo);
			mvwprintw(info,2,1,"Delay: %i ms",delay_ms);
		}

		if(!info_visible && info!=NULL){
			werase(info);
			destroy_win(info);
			info=NULL;
		}

		if(info!=NULL)
			wrefresh(info);

		//Aspetta qualche istante prima di cominciare un nuovo ciclo
		napms(delay_ms);

		//Incrementa il conto dei cicli
		ciclo++;
	}

	//Pulisce il terminale. Credo ci sia una funzione migliore nella libreria ncurses
	system("clear");

	//Termina la libreria ncurses
	endwin();
}

WINDOW *create_newwin(int height, int width, int starty, int startx)
{	WINDOW *local_win;

	local_win = newwin(height, width, starty, startx);
	box(local_win, 0 , 0);		/* 0, 0 gives default characters
					 * for the vertical and horizontal
					 * lines			*/
	wrefresh(local_win);		/* Show that box 		*/

	return local_win;
}

void destroy_win(WINDOW *local_win)
{
	/* box(local_win, ' ', ' '); : This won't produce the desired
	 * result of erasing the window. It will leave it's four corners
	 * and so an ugly remnant of window.
	 */
	wborder(local_win, ' ', ' ', ' ',' ',' ',' ',' ',' ');
	/* The parameters taken are
	 * 1. win: the window on which to operate
	 * 2. ls: character to be used for the left side of the window
	 * 3. rs: character to be used for the right side of the window
	 * 4. ts: character to be used for the top side of the window
	 * 5. bs: character to be used for the bottom side of the window
	 * 6. tl: character to be used for the top left corner of the window
	 * 7. tr: character to be used for the top right corner of the window
	 * 8. bl: character to be used for the bottom left corner of the window
	 * 9. br: character to be used for the bottom right corner of the window
	 */
	wrefresh(local_win);
	delwin(local_win);
}


void change_cell_color(){

	cl_color++;

	if(cl_color>=8)
		cl_color=0;

	initColorPair();

}

void change_bg_color(){

	bg_color++;

	if(bg_color>=8)
		bg_color=0;

	initColorPair();
}

void initColorPair(){
        init_pair(1, cl_color, bg_color);
}

void wipe_grid(){

	int x=0;
	int y=0;

	for(x=0; x<col; x++)
		for(y=0; y<row; y++)
			grid[x][y]=0;
}

void make_glider(int x, int y){

	int gilder[3][3]={{0,1,0},
			  {0,0,1},
			  {1,1,1}};

	int i=0;
	int j=0;

	for(i=0; i<3; i++)
		for(j=0; j<3; j++)
			grid[x+i][y+j]=gilder[i][j];
}

void copy_grid(){

	int x=0;
	int y=0;

	for(x=0; x<col; x++)
		for(y=0; y<row; y++)
			grid[x][y]=new_grid[x][y];

}

void fill_new_grid(){

	int i=0;

	for(i=1; i<=MAX_ROW; i++){
		//colonna sinistra in quella di destra
		grid[MAX_COL+1][i]=grid[1][i];
		//colonna destra in quella di sinistra
		grid[0][i]=grid[MAX_COL][i];
	}

	for(i=1; i<=MAX_COL; i++){
		//riga superiore in quella inferiore
		grid[i][MAX_ROW+1]=grid[i][1];
		//riga inferiore in quella superiore
		grid[i][0]=grid[i][MAX_ROW];
	}

	//i quattro angoli
	grid[0][0]=grid[MAX_COL][MAX_ROW];//inferiore destro in quello superiore sinistro
	grid[0][MAX_ROW+1]=grid[1][MAX_ROW];//inferiore sinistro in quello superiore destro
	grid[MAX_COL+1][MAX_ROW+1]=grid[1][1];//superiore sinistro in quello inferiore destro
	grid[MAX_COL][0]=grid[MAX_COL][1];//superiore destro in quello inferiore sinistro

}

void  make_new_grid(){

	int x=0;
	int y=0;

	for(x=0; x<col; x++)
		for(y=0; y<row; y++)
			new_grid[x][y]=check_life(x,y);
}


void make_random_grid(){

	srand(time(NULL));

	int x=0;
	int y=0;

	for(x=0; x<col; x++)
		for(y=0; y<row; y++)
			grid[x][y]=(rand()%2);

}

int check_life(int x, int y){

	int count=0;

	int a=0;
	int b=0;

	for(a=-1; a<2; a++){
		for(b=-1; b<2; b++){
			count=count+grid[x+a][y+b];
		}
	}

	count=count-grid[x][y];

	if(count==3 && !grid[x][y])
		return 1;
	else if ((count==2 || count==3) && grid[x][y])
		return 1;
	else return 0;

}

void draw_grid(){

	int x=0;
	int y=0;

	attron(COLOR_PAIR(1));
	attron(A_BOLD);

	for(x=1; x<col-1; x++){
		for(y=1; y<row-1; y++){
			if(grid[x][y]) {
				mvaddch(x,y,viva);
			}
			else {
                                mvaddch(x,y,morta);
			}
		}
	}

	attroff(A_BOLD);
	attroff(COLOR_PAIR(1));

	refresh();
}
