/**
 * @author Ayoub SANHAJI
 * @version 1.0
 */

#include <stdio.h>
#include <stdlib.h>
#include <glpk.h> /* l'entête de glpk */

void affiche(int *X)
{
  int i,j,k,n,temp,cmpt=0;
  int mat[10][10];
  /* conversion de l'index n de X(n)<>0 en index i,j,k */
  for(n=1;n<=729;n++){
    if(X[n]){
      cmpt++;
      temp=n-1;
      k=temp%9+1;
      temp=temp/9;
      j=temp%9;
      i=temp/9;
      mat[i][j]=k;
      }//if
    }//for i
    for(i=0;i<9;i++){
      if(i%3==0){
        printf("=============================\n");
      }//if
      for(j=0;j<9;j++){
        if(j%3==0){
         printf("|| ");
       }
       printf("%d ",mat[i][j]);
      }//for j
      printf("||\n");
    }//for i
    printf("=============================\n");
  }//affiche

  int main(int argc, char *argv[])
  { /* Déclarations */
    FILE *f; /*pointeur fichier avec les données du sudoku à résoudre*/
    LPX *lp; /* le problème */
    int i,j,k,i0,j0,numContrainte=0,numCoeffNz=0;
    int *ia; /* pointeur du vecteur contenant les indices des lignes des coefficients non nuls */
    int *ja; /* pointeur du vecteur contenant les indices des colonnes des coefficients non nuls */
    double *ar; /*pointeur du vecteur contenant les valeurs des coefficients non nuls */

    int nbFixes=0; /*le nb de valeurs connues dans la grille*/
    int nbContraintes; /*le nb de contraintes*/
    int nbCoeffNz; /*le nb de coefficients non nuls dans A*/
    int nbVariables=729; /*le nb de variables du vcteur X*/
    int X[nbVariables+1]; /* Déclaration du vecteur X*/


    /********************************************************************/
    /* lire le fichier de données
     *le nom du fichier de données est passé en argument sur la ligne de
     * commandes
     ********************************************************************/

  f=fopen(argv[1],"r");

    /* le premier élément du fichier est le nombre de cases fixes */
  fscanf(f,"%d",&nbFixes);

    /* on peut connaître maintenant le nombre de contraintes */
  nbContraintes = 324+nbFixes;

    // nbre des coeff en tout: 9*324+nbfixes
    /* et le nombre de coefficients non nuls de la matrice */
  nbCoeffNz = 2916+nbFixes;


  ia=(int*)malloc((nbCoeffNz+1)*sizeof(int));
  ja=(int*)malloc((nbCoeffNz+1)*sizeof(int));
  ar=(double*)malloc((nbCoeffNz+1)*sizeof(double));

  	/* création du problème */
    lp = glp_create_prob(); /* Voir manuel GLPK  */

    /* le nom du problème - facultatif */
    glp_set_prob_name(lp, "Sudoku");/* Voir manuel GLPK */

    /* c'est un MIP*/
    lpx_set_class(lp,LPX_MIP); /* Voir manuel GLPK */

    /* c'est un pb de maximisation */
    glp_set_obj_dir(lp,GLP_MAX);/* Voir manuel GLPK */

    /* le nom de la fonction objectif - facultatif */
  glp_set_obj_name(lp, "remplissage");

    /* on ajoute nbContraintes contraintes et nbVariables variables*/

  glp_add_rows(lp,nbContraintes);

  glp_add_cols(lp,nbVariables);


  	/* le type et le deuxième membre des contraintes -
       voir la doc de lpx_set_row_bnds */
  for(i=1;i<=nbContraintes;i++)
  {

  	glp_set_row_bnds(lp, i, GLP_FX, 1, 1);/* Voir manuel GLPK */

  }



    /* les bornes des variables - voir la doc de lpx_set_col_bnds */
  for(i=1;i<=nbVariables;i++)
  {

    glp_set_col_kind(lp, i, GLP_IV);
  //       /* Voir manuel GLPK */

    glp_set_col_bnds(lp, i, GLP_DB, 0.0, 1.0);
  //        Voir manuel GLPK 

  // 			/* On favorise le remplissage de toutes les cases de X*/
    glp_set_obj_coef(lp,i,1);
  //        /* Voir manuel GLPK   */

  }


    /* Gestion des contraintes : préparation de la matrice - voir manuel GLPK*/

  	/***************************************/
    /* un seul entier de 1 à  9 par case */
  	/****************************************/

  for(i=1;i<=9;i++)
  {
    for(j=1;j<=9;j++)
    {
     numContrainte++;
     for(k=1;k<=9;k++)
     {
      numCoeffNz++;
      ia[numCoeffNz] = numContrainte ;
      ja[numCoeffNz] = 81*(i-1)+9*(j-1)+k;
      ar[numCoeffNz] = 1.0;
  	     }//for k
  	 }//for j
       }//for i

    /******************************************************/
    /* chaque entier est une seule fois dans chaque ligne */
    /******************************************************/

       for(i=1;i<=9;i++)
       {
        for(k=1;k<=9;k++)
        {
         numContrainte++;
         for(j=1;j<=9;j++)
         {
           numCoeffNz++;
           ia[numCoeffNz] = numContrainte ;
           ja[numCoeffNz] = 81*(i-1)+9*(j-1)+k;
           ar[numCoeffNz] = 1.0;
  	    }//for k
  	}//for j
      }//for i
      


    /********************************************************/
    /* chaque entier est une seule fois dans chaque colonne */
    /********************************************************/

      for(j=1;j<=9;j++)
      {
        for(k=1;k<=9;k++)
        {
         numContrainte++;
         for(i=1;i<=9;i++)
         {
           numCoeffNz++;
           ia[numCoeffNz] = numContrainte ;
           ja[numCoeffNz] = 81*(i-1)+9*(j-1)+k;
           ar[numCoeffNz] = 1.0;
  	    }//for k
  	}//for j
      }//for i

    /**********************************************************/
    /* chaque entier est une seul fois dans chaque sous carré */
    //********************************************************
      for(k=1;k<=9;k++)
      {
        for(i0=0;i0<3;i0++)
        {
         for(j0=0;j0<3;j0++)
         {
           numContrainte++;
           for(i=1;i<=3;i++)
           {
            for(j=1;j<=3;j++)
            {
              numCoeffNz++;
              ia[numCoeffNz] = numContrainte ;
              ja[numCoeffNz] = 81*(3*i0+i-1)+9*(3*j0+j-1)+k;
              ar[numCoeffNz] = 1.0;
  		    }//for j
  		}//for i
  	    }//for j0
  	}//for i0
      }//for k

    /**************************/
    /* placer les cases fixes */
    /**************************/
      for(i=0;i<nbFixes;i++)
      {
        numContrainte++;
        numCoeffNz++;
        ia[numCoeffNz] = numContrainte ;
        fscanf(f,"%d",&i0);
        fscanf(f,"%d",&j0);
        fscanf(f,"%d",&k);
        ja[numCoeffNz] = 81*(i0-1)+9*(j0-1)+k;
        ar[numCoeffNz] = 1.0;
      }//for i

      fclose(f);

    /* chargement de la matrice */

  //printf("nbre contr: %d", numContrainte);
  	//*for(i=0; i<numCoeffNz; i++)
  		//printf("%d, ", ia[i]);*/

      glp_load_matrix(lp, numCoeffNz, ia, ja, ar);



  	/*FACULTATIF*/
  	/* écrire le problème dans un fichier pour voir s'il est bien créé */
    //glp_set_int_parm(lp, LPX_K_MPSORIG, 1);
    //glp_write_cpxlp(lp, "sudoku.lpt");


  	/* résoudre le problème par l'algo du simplexe */
        //lpx_simplex(lp,NULL);
        //lpx_integer(lp); /* Méthode SEP - Voir manuel GLPK*/
       lpx_intopt(lp); /* Méthode SEP - Voir manuel GLPK*/


    /* récupérer les résultats */
      int somme=0;
      for(i=1;i<=nbVariables;i++)
      {
        X[i]= glp_mip_col_val(lp, i); // Voir manuel GLPK
        somme+=X[i];
      }
      //for i


  	//Ecrire la solution dans le fichier
      affiche(X);
      int tmp=0,indice;
      FILE* fp2 = fopen("SOLUTION_SUDOKU.txt","w");
      for(indice=1;indice<=nbVariables;indice++)
      {
       tmp++;
       fprintf(fp2,"%d ",X[indice]);
       if(tmp%9==0) {
         tmp=0;
         fprintf(fp2,"\n");
       }
     }
     fclose(fp2);


  	/*glp_print_sens_bnds(lp,"sudoku.sens");*/  /*  Voir manuel GLPK*/


  	/* libérer la mémoire */
     glp_delete_prob(lp);
     free(ia);
     free(ja);
     free(ar);
     return 0;
   }


















