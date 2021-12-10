#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
    Nom : Halimi
    Prenom : Abdelkrim
    Groupe : 20
    Matricule : 19199DZA6012

    J'ai rassemblé les deux programmes en un seul fichier .exe pour faire un jolie programme qui peut a la fois
    demander la creation d'un fichier (le créer de A à Z ) ou le mettre a jour ( au choix ) , et lire ses questions si souhaitait , ou bien lire un fichier desiré
    sans avoir a le crée j'ai fais ça pour faire preuve d'originalité ! Merci bonne lecture !

*/

//-------------------------------------------------------------------- Prototypes fonctions
int creation();
int lancement(char *fichier_ouvrir,char rep[],int note,int choix);
int choisir(int choix,char nomFichier[]);
int verification(char fileName[],int *somme);
void input(char chaine[] , int taille);
void purgerBuffer();
//-------------------------------------------------------------------- Prototypes fonctions

int main()
{
    //------------------------------------------------------ Declaration Variable :
    char nom[100],prenom[100];
    int choix=0;
    char rep[400]="reponses_"; //--- chaine qui va être concatener pour le fichier des reponses
    int note=0;
    char nomFichier[100];
    //------------------------------------------------------ Programme :
    printf("== BIENVENUE SUR LE QUESTIONNAIRE DU MODULE D'ALGORITHMIQUE ==\n");
    printf("--Tout d'abord voulez vous creer/Mettre a jour  un questionnaire ou en lire un-- \n");
    printf("1)Creation/Maj \n");
    printf("2)Lire un Questionnaire \n");
    printf("3)Quitter le programme \n");
    //---------------------------------- choix entre créer un questionnaire ou en lire un
    scanf("%d",&choix);
    //---------------------------------- Appelle de la fonction " choisir()" qui return un "int" qui correspondra a la note du questionnaire crée si choix = 1
    note = choisir(choix,nomFichier);
    if(choix == 1)  //----------- Apres la creation du questionnaire on demande si il veut le lire ou pas, sinon on le redirige vers le debut du prog
    {
        printf("Voulez vous lire le questionnaire cree/mise a jour \n");
        printf("1)Oui\n2)Non\n\n");
        scanf("%d",&choix);
        if(choix == 1)
            printf("Lecture du questionnaire cree/maj...\n");
        else
        {
            printf("Vous allez etre rediriger vers la page d'Accueil ! \n ");
            main();
            return 0;
        }
    }
    //------------------------------------------------ Demande du nom et prenom
    printf("--> Veuillez preciser votre nom : ");
    scanf("%s",nom);
    purgerBuffer(); //----permet de vider le buffer avant pour pouvoir utiliser ma fonction "input " correctement sans bug  ( voir creation.c )
    printf("--> Veuillez preciser votre prenom : ");
    scanf("%s",prenom);
    purgerBuffer();
    printf("\n");
    printf("Merci ! Un fichier 'reponses_%s%s.txt' va etre cree pour mettre vos reponses et les noter !  \n\n",nom,prenom);
    //----------------------------------- Concatenation des chaines nom et prenom , puis rep et nom , ce qui correspond au nom du fichier crée pour les réponses.
    strcat(nom,prenom);
    strcat(rep,nom);
    //------------------------------------ si choix = 1 on lira le questionnaire crée , sinon le questionnaire par defaut
    if(choix == 1)
        lancement("questionnaire.txt",rep,note,choix); //-------- appelle de la fonction lancement qui prend ( le choix / la note du questionnaire / le nom fichier reponse et le Fichier reponse )
    else
        lancement(nomFichier,rep,note,choix);
    return 0;
}

//---------------------------------------------------------------------------------------------------------- Fonctions

int choisir(int choix,char nomFichier[]) //----- fonction qui permet de choisir entre la creation d'un questionnaire ou utiliser un par defaut
{
    int note=0;
    FILE *fichier = NULL;
    if(choix == 1)
        note = creation();   //--------------------------- si le choix = 1 on lancer la creation d'un questionnaire grace a la fonction creation() qui est dans le 2eme fichier.c
                          //-------------------------- et on recupere un int qu'on stock dans note ( qui sera la note total du bareme etabli )
    else if (choix == 2)    //------- sinon si le choix est le 2 , on demande a l'utilisateur un fichier a lire
    {
        printf("Vous avez choisit la lecture d'un fichier : \n");
        printf("Nom du fichier ? ( oubliez pas le .txt ) : \n");
        purgerBuffer();
        input(nomFichier,100); //------- fonction input que j'ai crée qui a le meme comportemenet que fgets ( voir creation.c )
        fichier = fopen(nomFichier,"r"); //---- on verifie que le fichier a lire existe
        if(fichier == NULL) //---------- si le fichier existe pas on appelle la fonction main() qui va reposer la question entre creation et lire un fichier
        {
            printf("Le Fichier en question n'existe pas dans votre programme!\nVeuillez choisir dans ce cas la creation d'un fichier merci ! \n");
            main();
            exit(0); //--- si tout s'est bien passé apres le main() on quitte
        }
    }
    else if (choix == 3)
        exit(0);
    else if ((choix != 2 && choix !=1 )&& (choix != 3))
    {
        printf("Choix non disponible !\n");
        printf("1)Creation/Maj \n");
        printf("2)Lire un Questionnaire \n");
        printf("3)Quitter le programme \n");
        scanf("%d",&choix);
        choisir(choix,nomFichier); //------------------- Sinon si on entre un nombre different de 1 et 2 la fonction se re appelle elle même
    }

    return note; //------ on return la note du bareme totale  qu'on a obtenue apres creation du questionnaire
}

int lancement(char *fichier_ouvrir,char rep[],int note,int choix) //---Fonction qui permet de lire le fichier crée ou par defaut afin de poser les questions et recuperer  les reponses dans un autrs fichier
{
    //------------------------------------------------------------------- Declaration Variable
    FILE* fichier= NULL;
    FILE* reponseFile = NULL;

    char caractere;
    int verification_fichier;
    int somme=0; //------- correspond a la somme des bonnes et mauvaise reponses de l'utilisateur
    int rajouter=0; //--- variable permettant la possibilité de rajouter une reponse ( utilisable dans  choix multiple )
    int reponse=0,reponse2=0,reponse3=0; //---- reponses de l'utilisateur
    int note2=0; //------------------------- correspond a la note du questionnaire totale ( si il a choisit d'en lire un )
    int resultat=0,resultat2=0,resultat3=0; //------ les resultats des reponses qui vont être mis dans la var somme
    char document[]=".txt";
    //------------------------------------------------------------------- Declaration Variable
    strcat(rep,document); //---- concatenation de rep et .txt pour crée le fichier reponses

    fichier = fopen(fichier_ouvrir,"r"); //------------ on lit le fichier qui a été crée ou bien le fichier qu'on veut lire  ça depend de l'utilisateur
    if(fichier == NULL) //---------- si le fichier existe pas on appelle la fonction main() qui va reposer la question entre creation et par defaut
    {
       printf("Le Fichier en question n'existe pas dans votre programme!\nVeuillez choisir dans ce cas la creation d'un fichier merci ! \n\n");
       main();
       return 0;
    }
    if(choix == 2) //--------------------- si on a choisit de lire un fichier sans creation on le verifie quand même au cas ou il est non eligible
    {
        verification_fichier = verification(fichier_ouvrir,&note2);
        if(verification_fichier == 0)
        {
            printf("\nQuestionnaire non Eligible ! \nVeuillez en lire un Existant/Eligible ou en creer un ! Merci\n");
            main();
            return 0;
        }
    }
    rename("reponse_user.txt",rep);      //-------------------- on crée un fichier reponse qui va être rename avec le bon nom
    reponseFile = fopen(rep,"w");       //---------------------- on ouvre le fichier qu'on vient de rename ( celui des reponses )

    if(fichier != NULL && reponseFile != NULL)  //- on verifie si l'ouverture des fichier a reussit ( ils devraient exister car ouvert precedemment en mode a ou w )
    {
        do
        {
            caractere = fgetc(fichier);    //------------------------ on lit caractere par caractere juusqu'a tomber sur les caracteres qui sont dans le switch.
            if((caractere != '~' && caractere != '^' )&& (caractere != '*'))  //-------- pour eviter d'afficher le caractere de reconaissance d'une question
                printf("%c",caractere);           //-------------------- on affiche dans la console tout les caracteres lu jusqu'au switch
            if(caractere != EOF)                  //--------------- et en même temps on ecrit les questions ( donc ce qu'on a lu ) dans le fichier reponses
                fprintf(reponseFile,"%c",caractere);
            switch(caractere) //---------- utilisation de caractere speciaux pour savoir qu'on commence la question de chaque type + leur bareme
            {
                case '~': //--------------------------------------- "~" signifie fin d'une question de type unique et debut du bareme de la question unique
                    do
                    {
                        printf("Votre reponse ? Choix Unique (1,2,3) : "); //----------- on lit la question puis demande la reponse tant que elle est valable
                        scanf("%d",&reponse);
                    }while(reponse>3 || reponse <1);

                        fprintf(reponseFile,"\nReponse %d\n~\n",reponse); //-------- on ecrit la reponse dans le fichier reponse
                        caractere = fgetc(fichier);

                    while(caractere != '~')  //---------------------- ce caractere delimite la fin du bloc ou est rangé le bareme de la question  dans le fichier
                    {
                        caractere = fgetc(fichier);
                        if((reponse == 1) && (caractere=='|'))  //------------ le caractere "|" signifie reponse 1 et le caractere juste apres correspond a aux points attribué a cette reponse
                            fscanf(fichier,"%d",&resultat); //----------------- on recupere la note de la reponse dans " resultat "

                        else if((reponse == 2 )&& (caractere ==',')) //------------- même chose ici le "," signifie reponse 2 et le caractere apres correspond aux points attribué acette reponse
                            fscanf(fichier,"%d",&resultat);

                        else if( (reponse == 3)&& (caractere =='.')) //---------- même chose ici
                            fscanf(fichier,"%d",&resultat);
                    }
                    somme+=resultat; //------------------------ correspond au score qu'on a eu dans cette question
                break;
                case '*': //--------------------------------------- caractere correspond a la fin  d'une question de type " bivalent " et debut de son bareme
                    do
                    {
                        printf("Votre reponse ? Choix Bivalent Vrai(1)/Faux(2) : "); //----------- on refait la même chose que pour les questions "Unique" juste ctte fois avec les questions bivalente
                        scanf("%d",&reponse);
                    }while(reponse>2 || reponse <1);
                        fprintf(reponseFile,"\nReponse %d\n*\n",reponse);
                        caractere = fgetc(fichier);
                    while(caractere != '*')  //--------------ce caractere delimite la fin du bloc ou est rangé le bareme de la question  dans le fichier
                    {
                        caractere = fgetc(fichier);
                        if((reponse == 1) && (caractere=='|'))
                            fscanf(fichier,"%d",&resultat);

                        else if((reponse == 2 )&& (caractere ==','))
                            fscanf(fichier,"%d",&resultat);
                    }
                    somme+=resultat;

                break;
                case '^': //--------------------------------------- caractere correspond a la fin  d'une question de type " multiple " et debut de son bareme
                    do
                    {
                        printf("Votre reponse Numero 1? Choix Multiple (1,2,3) : "); //---- on refait la même chose ici aussi c'est exactement pareil que pour les questions biavalente et unique sauf que on pose 2 reponses au moins
                        scanf("%d",&reponse);
                    }while(reponse>3 || reponse <1);
                        fprintf(reponseFile,"\nReponse %d\n",reponse);

                        printf("Votre reponse Numero 2? Choix Multiple (1,2,3) : "); //--- on demande une 2eme reponse differente de la 1er
                        scanf("%d",&reponse2);
                    while((reponse2>3 || reponse2<1) || (reponse2 == reponse))
                    {
                        printf("Votre reponse Numero 2 (differente de la 1er)? Choix Multiple (1,2,3) : ");
                        scanf("%d",&reponse2);
                    }

                    printf("Voulez vous rajouter une reponse ? : oui(1) / non(2) : "); //------------- Seulement cette fois on demande si il veut rajouter une 3eme reponse ou pas
                    scanf("%d",&rajouter);
                    if(rajouter != 1)
                        fprintf(reponseFile,"Reponse %d\n^\n",reponse2);
                    else if (rajouter == 1)
                    {
                        fprintf(reponseFile,"Reponse %d\n",reponse2);
                        do
                        {
                            printf("Votre reponse Numero 3 (differente de la 1er et 2eme )? Choix Multiple (1,2,3) : "); //--- reponse 3 differente des deux autres
                            scanf("%d",&reponse3);
                        }while(((reponse3>3 || reponse3<1) || (reponse3 == reponse)) || (reponse3 == reponse2)  );
                            fprintf(reponseFile,"Reponse %d\n^\n",reponse3);
                    }

                     caractere = fgetc(fichier);
                     while(caractere != '^') //--------------ce caractere delimite la fin du bloc ou est rangé le bareme de la question  dans le fichier
                    {
                        caractere = fgetc(fichier);
                        if((reponse == 1) && (caractere=='|')) //------------------------------------------------ pour la premire reponse
                            fscanf(fichier,"%d",&resultat);

                        else if((reponse == 2 )&& (caractere ==','))
                            fscanf(fichier,"%d",&resultat);

                        else if( (reponse == 3)&& (caractere =='.'))
                            fscanf(fichier,"%d",&resultat);

                        if((reponse2 == 1) && (caractere=='|')) //------------------------------------------------ pour la deuxieme reponse
                            fscanf(fichier,"%d",&resultat2);

                        else if((reponse2 == 2 )&& (caractere ==','))
                            fscanf(fichier,"%d",&resultat2);

                        else if( (reponse2 == 3)&& (caractere =='.'))
                            fscanf(fichier,"%d",&resultat2);

                        if(rajouter == 1)
                        {
                            if((reponse3 == 1) && (caractere=='|')) //------------------------------------------------ pour la troisieme reponse ( si y'en a une )
                                fscanf(fichier,"%d",&resultat3);

                            else if((reponse3 == 2 )&& (caractere ==','))
                                fscanf(fichier,"%d",&resultat3);

                            else if( (reponse3 == 3)&& (caractere =='.'))
                                fscanf(fichier,"%d",&resultat3);
                        }
                    }
                        if(rajouter == 1)
                            somme = somme + resultat2 + resultat + resultat3; //------------ on addittionne tout et on a donc le nombre de pts qu'on a obtenue dans somme
                        else
                            somme = somme + resultat2 + resultat;
                break;
            }

        }while (caractere != EOF); //--------- on lit tout les char different de EOF ( fin de fichier )


        printf("\nVous avez finis le questionnaire bien joue !! \n");
        if(choix == 1)  //------------------------------------------------------------Si c'était le questionnaire qu'on a crée on calcule le score grace a la somme des resultats sur la somme des pts positifs du bareme
        {
            printf("Voici votre note : %d/%d \n",somme,note); //---- note correspond a la premiere variable au tout debut qui correspond au points totaux du questionnaire
            fprintf(reponseFile,"\nNote : %d/%d",somme,note); //--- on ecrit la note obtenue dans le fichier reponse
        }
        else //------------------------------------------ Sinon avec le fichier de base qui est noté sur "14" on a juste a faire somme/14
        {
            printf("Voici votre note : %d/%d \n",somme,note2);
            fprintf(reponseFile,"\nNote : %d/%d ",somme,note2);
        }

        fclose(fichier); //--- fermeture fichier
        fclose(reponseFile);
    }
    else
    {
        printf("Erreur dans l'ouverture du fichier et dans la creation du fichier reponse !"); //------- si y'a une erreur on return -1 pour signaler
        return -1;
    }

    printf("\n---> Redirection vers le Menu : \n\n");
    main();
    return 0;
}

//----- Merci !! mtn voir fichier " creation.c "
/*
    Il faut savoir que dans mon fichier j'ai mis des caracteres speciaux specifiques pour savoir Quand est ce que on lit une question de Type Bivalent/Unique/Multiple
    Mais aussi Quand est ce que on lit un bareme de question de type Bivalent/unique/Multiple ce qui permet de se retrouver + facilement :

    Fin Question unique et Debut bareme unique  -> caractere '~'     et un 2eme '~' est placé juste apres pour marqué la fin du bareme unique
    Fin Question multiple et Debut bareme multiple  -> caractere '^'   et un 2eme '^'  est placé juste apres pour marqué la fin du bareme multiple
    Fin Question bivalent  et Debut bareme bivalent  -> caractere '*'    et un 2eme '*' est placé juste apres pour marqué la fin du bareme bivalent

    Bareme choix 1 -> caracetere '|'
    Bareme choix 2 -> caracetere ','
    Bareme choix 3 -> caracetere '.'

    Voila merci !

*/



