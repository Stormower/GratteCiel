#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <unistd.h>
#include "couleurs_terminal.h"
#include <time.h>
#include "saisie_avec_espaces.h"

//---------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------



//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~COULEURS TERMINAL~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

void clear_terminal()
{
  printf("\033[2J");
  printf("\033[0;0H");
}

int color_printf(COULEUR_TERMINAL fg, COULEUR_TERMINAL bg, const char * format, ...)
{
  int res;
  va_list args;
  printf("\x1B[1;%02d;%02dm", fg + 30, bg + 40);
  va_start(args, format);
  res = vprintf(format, args);
  va_end(args);
  printf("\x1B[00m");
  return res;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~COULEURS TERMINAL~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~SAISIE AVEC ESPACES~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

void purger()
{
  int c;
  while ((c = getc(stdin)) != '\n' && c != EOF)
      ;
}


void clean (char *chaine)
{
   char *p = strchr(chaine, '\n');
   if (p)
    *p = 0;
   else
    purger();
}

void saisie_avec_espaces(char * chaine, int n)
{
    fgets(chaine, n, stdin);
    clean(chaine);
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~SAISIE AVEC ESPACES~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~



typedef struct{
  int * grille;
  int n;
  int nb_cases_libres;
} Gratte_ciel;//création de la structure Gratte_ciel


Gratte_ciel* creer_gratte_ciel(int n)//Alouer dynamiquement un gratte ciel
{
  int i,j;
  if(n>=2 && n<=8) //arrete le programme si n n'est pas entre 2 et 8
  {
    Gratte_ciel *g1=NULL;// initialise le pointeur g1 a Null
    g1=(Gratte_ciel*)malloc(sizeof(Gratte_ciel)); // allocation de la memoire de la structure
    if(g1==NULL)
    {
      printf("Echec : Allocation Memoire\n");
      exit(-1);//en cas d'échec de l'allocation mémoire quitte le programme
    }
    g1->n=n;
    g1->nb_cases_libres=(n*n);
    g1->grille=(int*)malloc(((n+2)*(n+2))*sizeof(int));//reserve le nombre de cases necéssaire pour la grille
    if(g1==NULL)
    { 
      printf("Echec : Allocation Memoire\n");
      exit(-1);//pareil en cas d'échec d'allocation
    }
    for(i=0;i<(n+2);i++)//initialise toute les case a 0
    {
      for(j=0;j<(n+2);j++)
      {
        g1->grille[(i*(g1->n+2) +j)]=0;
      }
    }
    return g1;
  }
  printf("Echec : Taille incorrecte\n");
  return NULL;
}

void detruire_gratte_ciel(Gratte_ciel* p)//detruit un gratte ciel passé en parametre
{
  if(p!=NULL)//verifie que l'espace a detruire des bien aloué
  {
    free(p->grille);//Libère les cases de du tableau
    p->grille=NULL;//efface l'adresse
    free(p);//**
    p=NULL;//**
  }
}

int indice_grille_valide(Gratte_ciel * p, int i)
{
  if(p==NULL)
  {
    printf("Echec: Pointeur non alloué\n");
    exit(-1);
  }
  if(i>=0 && i<=(p->n)+1)//verifie que 'indice' est entre 0 et n+1
    return 1;
  return 0;
}

int get_case(Gratte_ciel *p, int i, int j)
{
  if (!indice_grille_valide(p,i) || !indice_grille_valide(p,j))
    return -1;
  else
    return p->grille[(i*(p->n+2)+j)];//retourne le contenu de la case
}

int set_case(Gratte_ciel * p,int i,int j,int val)
{
  if((indice_grille_valide(p,i)&& indice_grille_valide(p,j)) && (val>=0 && val<=p->n))//verifie que les indices i j sont valide
  {
    if((i>0 && i<p->n+1)&&(j>0 && j<p->n+1))//verifie si la case appartient bien au quartier
    {
      if((get_case(p,i,j)==0) && (val!=0))//verifie si la case etait vide
          p->nb_cases_libres--;//met a jour le nombre de case vide
      if((get_case(p,i,j)!=0) && val==0)//
        p->nb_cases_libres++;//
    }
    p->grille[(i*(p->n+2) +j)]=val;//attribue val a la case donnée
    return 1;
  }
  return 0;
}



int get_nb_cases_libres(Gratte_ciel *p)
{
  if(p==NULL)
  {
    printf("Echec: Pointeur non alloué\n");
    exit(-1);
  }
  return p->nb_cases_libres;
}


int est_case_bordure(Gratte_ciel * g, int i, int j)//retourne 1 si la case appartient a la bordure
{
  if(g==NULL)
  {
    printf("Echec: Pointeur non alloué\n");
    exit(-1);
  }
  if(((i>0 && i<g->n+1) && (j==0 || j==g->n+1)) || ((i==0 || i==g->n+1) && (j>0 && j<((g->n)+1))))
  {
    return 1;
  }
  return 0;
}

/*void affichage_gratte_ciel(Gratte_ciel *p)//affichage simple
{
  int i=0;
  int j;  
  for(i=0; i< p->n+2; i++)//affiche les caractères (n par ligne)
  {
      for(j=0; j< p->n+2; j++)//affiche la valeur pour une coordonée puis passe a la suivante
        {
          printf("%d", get_case(p,i,j));
          printf(" ");
      }
      printf("\n");
  }
}*/

void affichage_gratte_ciel(Gratte_ciel * p)
{
  clear_terminal();
  int i;
  int j;
  int a=0;
  if(p==NULL)
  {
    printf("Echec: Pointeur non alloué\n");
    exit(-1);
  }
  for (i= 0; i<p->n+2;i++)//Boucle qui affiche les indices des colonnes
  {
    if ((i==0 || i==p->n+1))
            printf("          ");
        else
          printf("    %d    ",i);
  }
  printf("\n");
  for(i=0; i< p->n+2; i++)//Boucle pour le tableau entier
  {
    for(j=0; j< p->n+2; j++)//pour rendre la grille classe(carré parfait)
       {
        if (((i==0 || i==p->n+1) && !(j>0 && j<p->n+1)))
            printf("         ");
        else if(est_case_bordure(p,i,j))
        { 
          printf(" ");
          color_printf(MAGENTA,MAGENTA,"        ");
        }
        else
        {
          printf(" ");
          color_printf(BLUE,BLUE,"        ");
        }
       }
       printf("\n");
      for(j=0; j< p->n+2; j++)//affiche les valeur pour une ligne en jolie jolie
        {
          if((i>0 && i<p->n+1) && j==0)//affiche indice ligne(A B C ...)
          {
            printf("%c",i+64);
            a++;  
          }
          if (((i==0 || i==p->n+1) && (j==0 || j==p->n+1)))//si la case est un rebord non utilisé
            printf("         ");
          else if(est_case_bordure(p,i,j))//si la case appartient a la bordure
            color_printf(WHITE,MAGENTA,"   %2.d   ", get_case(p,i,j));
          else// si c'est une case centrale
            color_printf(WHITE,BLUE,"   %2.d   ",get_case(p,i,j)*10);
          printf(" ");
      }
      printf("\n");
      for(j=0; j< p->n+2; j++)//pour rendre la grille classe(carré parfait)
      {
        if (((i==0 || i==p->n+1) && !(j>0 && j<p->n+1)))
            printf("         ");
        else if(est_case_bordure(p,i,j))
        {
          printf(" ");
          color_printf(MAGENTA,MAGENTA,"        ");
        }
        else
        {
          printf(" ");
          color_printf(BLUE,BLUE,"        ");
        }
      }
    printf("\n\n"); 
  }
}


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~PARTIE 2~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~



/*est_case_bordure -----> PARTIE 1*/


int nb_immeubles_visibles(Gratte_ciel * p,int i,int j)//fonction qui retourne le nombre d'immeubles visible depuis une case bordure
{
  if(est_case_bordure(p,i,j))//verifie que la case appartiens bien a la bordure
  {
    int cmpt=1;
    int tmp1,tmp2;
    if(i==0)//1er des 4 cas // si la case est en haut
    {
      i=1;
      tmp1=get_case(p,i,j);//stock le premier immeubles visible depuis la case(plus grand par defaut)
      while((i+1<=p->n))//tant que l'immeuble suivant appartient a la ligne
      {
        tmp2=get_case(p,i+1,j);//l'immeuble suivant devient tmp2
        if(tmp1<tmp2)//si tmp2 est plus grand que l'immeuble courrant
        {
          cmpt++;//met a jour le nombre d'immeuble visible
          tmp1=tmp2;//l'immeuble le plus grand visible deviens l'immeuble courrant
        }
        i++;  
      }
    }
    if(i==p->n+1)//cas ou la case est en bas
    {
      i=p->n;//
      tmp1=get_case(p,i,j);
      while((i-1>=1))
      {
        tmp2=get_case(p,i-1,j);
        if(tmp1<tmp2)
        {
          cmpt++;//met a jour le nombre d'immeuble visible
          tmp1=tmp2;//l'immeuble le plus grand visible deviens l'imm. courrant
        }
        i--;
      }
    }
    if(j==0)//cas ou la case est a gauche
    {
      j=1;
      tmp1=get_case(p,i,j);//stock le premier immeubles visible depuis la case(plus grand par defaut)
      while((j+1<=p->n))//tant que l'immeuble suivant appartient a la ligne
      {
        tmp2=get_case(p,i,j+1);//l'immeuble suivant devient tmp2
        if(tmp1<tmp2)//si tmp2 est plus grand que l'immeuble courrant
        {
          cmpt++;//met a jour le nombre d'immeuble visible
          tmp1=tmp2;//l'immeuble le plus grand visible deviens l'immeuble courrant
        }
        j++;  
      }
    }
    if(j==p->n+1)//cas ou la case est a droite
    {
      j=p->n;//
      tmp1=get_case(p,i,j);
      while((j-1>=1))
      {
        tmp2=get_case(p,i,j-1);
        if(tmp1<tmp2)
        {
          cmpt++;//met a jour le nombre d'immeuble visible
          tmp1=tmp2;//l'immeuble le plus grand visible deviens l'imm. courrant
        }
        j--;
      }
    }
    if(!tmp1)//si a la fin tmp=0--->il n'y a pas d'immeuble sur la ligne/col
      return tmp1;//donc retourn 0 (0 immeubles visible depuis la case)
    return cmpt;
  }
  return -1;//ce n'est pas une case bordure
}

void calcul_bordure(Gratte_ciel *p) //fonction qui met a jour tout les immeubles visible depuis une case
{
  if(p==NULL)
  {
    printf("Echec: Pointeur non alloué\n");
    exit(-1);
  }
  int i,j;
  for(i=0;i<(p->n+2);i++)//parcours le tableau case par case
  {
    for(j=0;j<(p->n+2);j++)
    {
      if(est_case_bordure(p,i,j))//a chaque case bordure met a jour le nombre d'immeuble visible
        set_case(p,i,j,nb_immeubles_visibles(p,i,j));
    }
  }
}


int valeurs_differentes_ligne ( Gratte_ciel * p, int ind)
{
  if(p==NULL)
  {
    printf("Echec: Pointeur non alloué\n");
    return 0;
  }
  int i,j;
  for(i=1;i<=p->n;i++)
  {
    j=i+1;
    for(;j<=p->n;j++)
    {
      if(get_case(p,ind,i)==get_case(p,ind,j))
        return 0;
    }
  }
  return 1;
}



int valeurs_differentes_colonne ( Gratte_ciel * p, int ind)
{
  if(p==NULL)
  {
    printf("Echec: Pointeur non alloué\n");
    return 0;
  }
  int i,j;
  for(i=1;i<=p->n;i++)
  {
    j=i+1;
    for(;j<=p->n;j++)
    {
      if(get_case(p,i,ind)==get_case(p,j,ind))
        return 0;
    }
  }
  return 1;
}


int valeurs_differentes_quartier ( Gratte_ciel * p)
{
  if(p==NULL)
  {
    printf("Echec: Pointeur non alloué\n");
    return 0;
  }
  int i;
  for(i=1;i<=p->n;i++)
  {
    if(!valeurs_differentes_ligne(p,i))
      return 0;
  }
  for(i=1;i<=p->n;i++)
  {
    if (!valeurs_differentes_colonne(p,i))
      return 0;
    
  }
  return 1;
}



int bordure_correcte(Gratte_ciel * p)
{
  if(p==NULL)
  {
    printf("Echec: Pointeur non alloué\n");
    return 0;
  }
  int i,j;
  for(i=0;i<p->n+1;i++)
  {
    for (j=0; i <p->n+1;i++)
    {
      if(est_case_bordure(p,i,j) && (get_case(p,i,j)!=nb_immeubles_visibles(p,i,j)) && get_case(p,i,j))
        return 0;
    }
  }
  return 1;
}


int quartier_est_solution ( Gratte_ciel * p)
{
  if((!(get_nb_cases_libres(p))) && valeurs_differentes_quartier(p) && bordure_correcte(p))
    return 1;
  return 0;
}



//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~FONCTIONS ADDITIONNELLES~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~


int get_size(Gratte_ciel *p)
{
  if(p==NULL)
  {
    printf("Echec: Pointeur non alloué\n");
    return -1;
  }
  return p->n;
}



int indice_quartier_valide(Gratte_ciel *p,int i)
{
  return (i>0 && i<=get_size(p)) ? 1 : 0;
}

void permute_cases(Gratte_ciel * p,int i1,int j1,int i2,int j2)
{
  int a=get_case(p,i1,j1);
  set_case(p,i1,j1,(get_case(p,i2,j2)));
  set_case(p,i2,j2,a);
}


void message()
{
    color_printf(WHITE,BLACK,"       ######                                                           ");
  printf("\n");
  color_printf(WHITE,BLACK,"       #     # # ###### #    # #    # ###### #    # #    # ######       ");
  printf("\n");
  color_printf(WHITE,BLACK,"       #     # # #      ##   # #    # #      ##   # #    # #            ");
  printf("\n");
  color_printf(WHITE,BLACK,"       ######  # #####  # #  # #    # #####  # #  # #    # #####        ");
  printf("\n");
  color_printf(WHITE,BLACK,"       #     # # #      #  # # #    # #      #  # # #    # #            ");
  printf("\n");
  color_printf(WHITE,BLACK,"       #     # # #      #   ##  #  #  #      #   ## #    # #            ");
  printf("\n");
  color_printf(WHITE,BLACK,"       ######  # ###### #    #   ##   ###### #    #  ####  ######       ");                                                        
  printf("\n");
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~FONCTIONS ADDITIONNELLES~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~


void permute_lignes(Gratte_ciel * p, int l1, int l2)
{
  if(l1!=l2 && indice_quartier_valide(p,l1) && indice_quartier_valide(p,l2))
  {
    int i,n;
    n=get_size(p);
    for(i=1;i<=n;i++)
      permute_cases(p,l1,i,l2,i);
  }
}


void permute_colonnes(Gratte_ciel * p, int c1, int c2)
{
  if(c1!=c2 && indice_quartier_valide(p,c1) && indice_quartier_valide(p,c2))
  {
    int i,n;
    n=get_size(p);
    for(i=1;i<=n;i++)
      permute_cases(p,i,c1,i,c2);
  }
}

void permute_nombres(Gratte_ciel * p, int nb1, int nb2)
{
  if(!get_nb_cases_libres(p) && nb1!=nb2  && indice_quartier_valide(p,nb1) && indice_quartier_valide(p,nb2))
  {
    int i,j,n=get_size(p);
    for(i=1;i<=n;i++)
    {
      for(j=1;j<=n;j++)
      {
        if(get_case(p,i,j)==nb1)
          set_case(p,i,j,nb2);
        else if(get_case(p,i,j)==nb2)
          set_case(p,i,j,nb1);
      }
    }
  }
}



void remplir_quartier( Gratte_ciel * p)
{
  int i,j,n=get_size(p);
  for(i=1;i<=n;i++)
  {
    for(j=1;j<=n;j++)
    {
      if(i==j)
        set_case(p,i,j,1);
      if (j>i)
        set_case(p,i,j,get_case(p,i,j-1)+1);
    }
  }
  for(i=1;i<=n;i++)
  {
    for(j=1;j<=n;j++)
    {
      if(j==1 && i!=1)
        set_case(p,i,j,get_case(p,i,n)+1);
      if (j>1 && j<i)
        set_case(p,i,j,get_case(p,i,j-1)+1);
    }
  }
}



int nombre_aleatoire(int min, int max)
{
  if(min<=max)
    return (rand()%((max+1)-min))+min;
  return -1;
}



void quartier_aleatoire ( Gratte_ciel * p)
{
  remplir_quartier(p);
  int i,n,perm,size=get_size(p);
  n=nombre_aleatoire(50,100);
  for(i=0;i<n;i++)
  {
    perm=nombre_aleatoire(1,3);
    if (perm==1)
      permute_colonnes(p,nombre_aleatoire(1,size),nombre_aleatoire(1,size));
    if(perm==2)
      permute_lignes(p,nombre_aleatoire(1,size),nombre_aleatoire(1,size));
    if(perm==3)
      permute_nombres(p,nombre_aleatoire(1,size),nombre_aleatoire(1,size));
  }
  affichage_gratte_ciel(p);
}


Gratte_ciel * creer_gratte_ciel_aleatoire (int n)
{
  Gratte_ciel * g=creer_gratte_ciel(n);
  int i,j;
  quartier_aleatoire(g);
  calcul_bordure(g);
  for(i=1;i<=n;i++)
  {
    for(j=1;j<=n;j++)
      set_case(g,i,j,0);
  }
  return g;
}


int mouvement(Gratte_ciel * p)
{
  char mvt[5];
  do
  {
    printf("Saisir un mouvement\n");
    saisie_avec_espaces(mvt,5);
    if(mvt[0]=='s' && mvt[1]=='t' && mvt[2]=='o' && mvt[3]=='p')
      return 0;
  }while((!indice_quartier_valide(p,mvt[0]-64) && !indice_quartier_valide(p,mvt[1]-48) && (!indice_quartier_valide(p,mvt[3]-48) && mvt[3]-48!=0)));
  set_case(p,(int)mvt[0]-64,(int)mvt[1]-48,(int)mvt[3]-48);
  return 1;
}

void jouer(int n)
{
  Gratte_ciel * g=creer_gratte_ciel_aleatoire(n);
  affichage_gratte_ciel(g);
  while(mouvement(g) && get_nb_cases_libres(g))
  {
    affichage_gratte_ciel(g);
  }
  if (!get_nb_cases_libres(g))
  {
    if(quartier_est_solution(g))
    {
      affichage_gratte_ciel(g);
      color_printf(WHITE,GREEN,"                                 Partie Gagnée !                              \n");
    }
    else
    {
      affichage_gratte_ciel(g);
      color_printf(WHITE,RED,"                                  Partie Perdu !                               \n");
    }
  }
  else
    printf("Jeu arrêté\n");
  detruire_gratte_ciel(g);
}

int main()
{
  srand(time(NULL));
  int n;
  message();
  do
  {
    printf("\n\nVeulliez Saisir la Taille de la Grille (2-8) \n");
    scanf("%d",&n);
  }while(n>8 && n<2);
  jouer(n);
  printf("\n\n~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~Partie Terminé~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
  return 0;
}