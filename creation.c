#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
    Nom :
    Prenom :
    Groupe :
    Matricule :

    J'ai rassemblé les deux programmes en un seul fichier .exe pour faire un jolie programme qui peut a la fois
    demander la creation d'un fichier (le créer de A à Z ) ou le mettre a jour ( au choix ) , et lire ses questions si souhaitait , ou bien lire un fichier desiré
    sans avoir a le crée j'ai fais ça pour faire preuve d'originalité ! Merci bonne lecture !

*/

//------------------------------------ Declaration et definition des structures pour + de faicilté
typedef struct Type Type;
typedef struct Questionnaire Questionnaire;
struct Type
{
    char unique[200];   //--------- type unique
    char multiple[200]; //--------- type multiple ( pas de type bivalent car suffit d'écrire Vrai ou Faux )
};
struct Questionnaire
{
    char question[1000]; //----- question a poser
    Type choix;          //---------- son type
    int bareme;          //----------- le bareme
    int somme;           //-------------- la somme du bareme
};

//----------------------------- Prototypes fonction :

int creation();
void input(char chaine[], int taille);
void rechercher(char chaine[]);
void purgerBuffer();
void questionnaire_eligible();
void unique(Questionnaire *question, FILE *fichier);
void multiple(Questionnaire *question, FILE *fichier);
void bivalent(Questionnaire *question, FILE *fichier);
void bareme(Questionnaire *question, FILE *fichier);
void bareme_Bivalent(Questionnaire *question, FILE *fichier);
int verification(char fileName[], int *somme);

//----------------------------- Prototypes fonction :

//---------------------------------------------------------------- Fonctions du code

int creation()
{
    //---------------------------------------------------------- Declaration Variable :
    FILE *fichier = NULL;
    Questionnaire question; //-------------- variable questionnaire
    int nbr_question = 0, i = 0;
    int verification_fichier = 0; //---------- pour verifier le fichier si il est bon ou pas
    int type = 0;
    int maj = 0; //---- savoir si on fait une MAJ ou pas
    int eligible = 0;
    char nomFichier[] = "questionnaire.txt";
    char nomFichierMaj[100]; //--------------fichier qu'on veut mettre a jour si demander
    int sommation = 0;       //---------------------- pour la somme des pts du bareme

    //---------------------------------------------------------- Declaration Variable :

    printf("BIENVENUE DANS LA CREATION D'UN QUESTIONNAIRE\n");
    do //------ On repose la question tant que le choix est pas valide
    {
        printf("Voulez vous voir comment faire un questionnaire eligible ? Oui(1) / Non(2) :"); //--- Ceci explique comment faire un questionnaire eligible
        scanf("%d", &eligible);
        purgerBuffer(); //----- permet de vider le buffer afin de pouvoir ressaisir une chaine de caracteres

    } while (eligible != 1 && eligible != 2);
    //------( je l'utilise pour pouvoir utliser ma fonction "input" sans probleme )                                                                                                         //---- ma fonction input a le comportement de fgets
    if (eligible == 1)
        questionnaire_eligible(); //---- Appelle de la fonction qui explique cela
    do                            //------------------------------------- On repose la question tant que le choix est pas valide
    {
        printf("Voulez vous creer un fichier , ou en mettre a jour un ? : Creation(1)/ MAJ(2): "); //--- demande si on veut créer ou Mettr a jour un fichier
        scanf("%d", &maj);
        purgerBuffer();

    } while (maj != 2 && maj != 1);

    if (maj == 2) //--------- si il veut faire une MAJ
    {
        printf("Nom du fichier ?  ( oubliez pas le .txt ) : \n");
        input(nomFichierMaj, 100);           //------ on demande le nom du fichier a mettre a jour, on utilise ma fonction "input" une dérivé de fgets que j'ai crée
        fichier = fopen(nomFichierMaj, "r"); //---------- on ouvre le fichier en mode " r " pour savoir si il existe ou pas
        if (fichier == NULL)
            printf("Fichier innexistant -> Creation d'un fichier ! \n"); //--si il existe pas on va en crée un en l'ouvrant en mode "a" car on veut faire une MAJ dans tout les cas
        fclose(fichier);
    }
    else
        printf("Creation d'un fichier 'questionnaire.txt' ... \n");

    printf("Combien vous avez de question ? : \n"); //----- on demande le nombre de question
    scanf("%d", &nbr_question);
    purgerBuffer();
    if (maj == 2)
        fichier = fopen(nomFichierMaj, "a"); //------------ si on a choisit de mettre a jour un fichier
    else
        fichier = fopen(nomFichier, "w"); //----------- ouverture fichier en mode ecriture si on choisit d'en créer un

    if (fichier != NULL) //------------------------------------ verification de si l'ouverture a reussit ( et si il existe ) sinon erreur
    {                    //------- normalement le fichier existe car en mode " w " ou "a" ça en crée un automatiquement
        for (i = 0; i < nbr_question; i++)
        {
            printf("Question Numero %d : ", i + 1);
            input(question.question, 1000); //------ on demande la question a poser, on utilise ma fonction "input"
                                            //--------- qui copie le comportement de fgets , sauf que elle vide le buffer et supp le retour a la ligne
            fprintf(fichier, "\nQuestion: %s\n", question.question);

            do //------ tant que on a pas le type on redemande
            {
                printf("Type de la Question : \n1-Choix Unique\n2-Choix Multiple\n3-Choix Bivalent \n"); //------------ on demande son type
                scanf("%d", &type);
                purgerBuffer();
                if (type == 1)
                    unique(&question, fichier); //---------- si unique on appelle la fonction unique , meme chose pour les autres
                else if (type == 2)
                    multiple(&question, fichier); //---- appelle fonction mutliple (on fait un passage par adresse pour eviter la copie et modifier directement la variable)
                else if (type == 3)
                    bivalent(&question, fichier); //----- appelle fonction bivalent
                else
                    printf("Il y'a pas d'autres choix desole ! \n");

            } while (type < 1 || type > 3);
        }
        fclose(fichier); //---- on ferme le fichier a la fin bien sur
    }
    else
    {
        printf("Erreur ouverture fichier ! \n");
        fclose(fichier);
        return -1;
    }

    if (maj == 2)                                                       //--------------- verification , permet de verifier le fichier si il est eligible ou pas ça return 1 pour true et 0 pour false
        verification_fichier = verification(nomFichierMaj, &sommation); //------- on envoie l'adresse de la somme qui va être modifier pour correspondre au pts du bareme totale
    else                                                                //---- on verifie soit le fichier crée soit le fichier mis a jour
        verification_fichier = verification(nomFichier, &sommation);

    if (verification_fichier == 1)
        printf("Questionnaire Eligible ! \n");
    else if (verification_fichier == 0)
    {
        printf("\n\nQuestionnaire non Eligible !\nVeuillez en re creer un ! \n\n");
        creation(); //-------------------------------------- si le questionnaire est pas eligible on redemande la creation
    }
    question.somme = sommation;
    return question.somme; //-------- d'ou note = creation() dans le 1er fichier car ça retourne la somme des pts du bareme
}

void input(char chaine[], int taille) //------- permet d'avoir le comportement de fgets, sauf que le retour a la ligne et la suppression du buffer est automatisé
{
    if (fgets(chaine, taille, stdin) != NULL) //---- si la chaine existe on cherche le caractere de fin et on le supprime, si il existe pas on purge
        rechercher(chaine);
    else
        purgerBuffer(); //-------- si la chaine existe pas on purge aussi
}

void rechercher(char chaine[]) //----- permet de supprimer le retour a la ligne de fgets
{
    char *retour_ligne = strchr(chaine, '\n'); //--- on cherche le retour a la ligne dans la phrase
    if (retour_ligne)
        *retour_ligne = '\0'; //--- on le remplace par le caractere de fin
    else
        purgerBuffer(); //--- sinon on purge
}

void purgerBuffer() //----- permet de vider le buffer afin de pouvoir ressaisir une chaine de caracteres
{
    int vider = 0;
    while (vider = getchar() != '\n' && vider != EOF)
    {
    }
    //---- on lit char par char tant que on est pas arrivé a un des 2 char cité qui marque qu'on est a la fin du buffer
}

void questionnaire_eligible() //------------- permet d'expliquer ce qu'est un questionnaire eligible :
{
    printf("\nVoici comment faire un questionnaire eligible : \n");
    printf("-Au moins 3 questions \n");
    printf("-Utilisation de 3 Types de questions au minumum : \n1)Choix Unique\n2)Choix Multiple\n3)Choix Bivalent \n");
    printf("-Exactement une note positive pour les questions a choix unique et choix bivalent \n");
    printf("-Au moins 2 notes positives pour les questions a choix multiple \n");
    printf("-Merci !\n\n\n");
}

void unique(Questionnaire *question, FILE *fichier) //-----------Fonction permettant de demander a l'utilisateur d'entrer des choix de reponse de type  " unique "
{
    printf("Choix Numero 1 : \n");                      //------------------ d'ailleur on fait question->choix.unique car question est un pointeur , et unique est un tableau de char
    input(question->choix.unique, 200);                 //---------------------- on écrit le choix 1 (même chose pour choix 2 et 3 )
    fprintf(fichier, "1)%s\n", question->choix.unique); //--------- et on l'ecrit dans le fichier questionnaire.txt
    printf("Choix Numero 2 : \n");
    input(question->choix.unique, 200);
    fprintf(fichier, "2)%s\n", question->choix.unique);
    printf("Choix Numero 3 : \n");
    input(question->choix.unique, 200);
    fprintf(fichier, "3)%s\n", question->choix.unique);
    fprintf(fichier, "~");     //---------------------------- on crée un bloc ou se trouve le bareme delimité par " ~ "
    bareme(question, fichier); //--------- appelle de la fonction pour etablir un bareme des questions unique
    fprintf(fichier, "~");
}

void multiple(Questionnaire *question, FILE *fichier) //-----------Fonction permettant de demander a l'utilisateur d'entrer des choix de reponse de type  " multiple "
{
    printf("Choix Numero 1 : \n");                        //------------------ d'ailleur on fait question->choix.unique car question est un pointeur , et unique est un tableau
    input(question->choix.multiple, 200);                 //---------------------- on écrit le choix 1 (même chose pour choix 2 et 3 )
    fprintf(fichier, "1)%s\n", question->choix.multiple); //--------- et on l'ecrit dans le fichier questionnaire.txt
    printf("Choix Numero 2 : \n");
    input(question->choix.multiple, 200);
    fprintf(fichier, "2)%s\n", question->choix.multiple);
    printf("Choix Numero 3 : \n");
    input(question->choix.multiple, 200);
    fprintf(fichier, "3)%s\n", question->choix.multiple);
    fprintf(fichier, "^");     //---------------------------- on crée un bloc ou se trouve le bareme delimité par " ^ "
    bareme(question, fichier); //--- appelle de la fonction pour etablir un bareme des questions multiple
    fprintf(fichier, "^");
}

void bivalent(Questionnaire *question, FILE *fichier) //-----------Fonction permettant de demander a l'utilisateur d'entrer des choix de reponse de type  " Bivalent "
{
    fprintf(fichier, "1)Vrai\n2)Faux\n"); //------ on ecrit nous même " Vrai ou Faux " dans le fichier pas besoin de demander a l'utilisateur
    fprintf(fichier, "*");                //---------------------------- on crée un bloc ou se trouve le bareme delimité par " * "
    bareme_Bivalent(question, fichier);   //--- appelle de la fonction pour etablir un bareme des questions de type bivalent
    fprintf(fichier, "*");
}

void bareme(Questionnaire *question, FILE *fichier) //---------- Fonction permettant d'établir un bareme pour la question unique ou multiple
{                                                   //--- le bareme sera ecrit dans le fichier entre 2 caracteres speciaux pour chaque type de question pour eviter qu'il soit lu
    printf("Points choix 1 ?: ");
    scanf("%d", &question->bareme); //---- on demande le nombre de pt pour le choix 1 puis 2 et  3
    purgerBuffer();
    fprintf(fichier, "\n|%d,", question->bareme); //--- on range le bareme du choix 1 apres le caractere "|" par exemple ici et le choix 2 apres "," 3 apres "."
    printf("Points choix 2 ?: ");
    scanf("%d", &question->bareme);
    purgerBuffer();
    fprintf(fichier, "%d.", question->bareme);
    printf("Points choix 3 ?: ");
    scanf("%d", &question->bareme);
    purgerBuffer();
    fprintf(fichier, "%d\n", question->bareme);
}

void bareme_Bivalent(Questionnaire *question, FILE *fichier) //--- même chose ici sauf que ça concerne que les questions bivalente
{
    printf("Points Vrai ?: ");
    scanf("%d", &question->bareme); //---- on demande le nombre de pt pour le choix 1 puis 2
    purgerBuffer();
    fprintf(fichier, "\n|%d,", question->bareme);
    printf("Points Faux ?: ");
    scanf("%d", &question->bareme);
    purgerBuffer();
    fprintf(fichier, "%d\n", question->bareme);
}

int verification(char fileName[], int *somme) //------ fonction qui permet de verifier si un fichier est eligible ou non (prends un pointeur sur somme pour modifier la note totale directement)
{
    //-------------------------------------------------- Declaration Variable
    char caractere;
    FILE *fichier = NULL;
    fichier = fopen(fileName, "r");
    int reponse_unique = 0;
    int reponse_multiple = 0;
    int reponse_bivalent = 0;
    int fichier_non_eligible = 0;
    int resultat = 0;
    int nbr_qst_biv = 0;
    int nbr_qst_uniq = 0;
    int nbr_qst_mul = 0;
    //-------------------------------------------------- Declaration Variable

    if (fichier != NULL) //--------------- on verifie si l'ouverture du fichier transmit a reussit ou pas ( il devrait existait car ouvert precedemment en mode a ou w )
    {
        do
        {
            caractere = fgetc(fichier); //------------ on lit char par char
            switch (caractere)          //----------------------- on switch "caracetere" car chaque type de question est delimité par un caracetere speciale et son bareme aussi
            {
            case '~':           //------------------------------- "~" signifie fin de question unique et debut de bareme de la question unique
                nbr_qst_uniq++; //--------- incremente a chaque fois que une question de type unique est trouvable dans le fichier
                caractere = fgetc(fichier);
                while (caractere != '~') //---------------- le caractere delimite aussi le bloc ou se trouve le bareme de la question
                {
                    caractere = fgetc(fichier);
                    if (caractere == '|') //-------------caracete signifiant debut de la note attribué au choix 1
                    {
                        fscanf(fichier, "%d", &resultat); //----------- on recupere cette note
                        if (resultat > 0)                 //------------ on check si elle est positive , ( il doit y'avoir que une d'ou l'incrementation qui doit pas depasser 1 )
                        {
                            *somme += resultat; //---- si le bareme est positif on l'ajoute a somme qui correspond a la note totale du questionnaire
                            reponse_unique++;
                        }
                    }
                    else if (caractere == ',') //-------------caracete signifiant debut de la note attribué au choix 2
                    {
                        fscanf(fichier, "%d", &resultat); //--- on refait la même que pour le choix 1
                        if (resultat > 0)
                        {
                            *somme += resultat;
                            reponse_unique++;
                        }
                    }

                    else if (caractere == '.') //-------------caracete signifiant debut de la note attribué au choix 3
                    {
                        fscanf(fichier, "%d", &resultat); //--- on refait la même que pour le choix 1 et 2
                        if (resultat > 0)
                        {
                            *somme += resultat;
                            reponse_unique++;
                        }
                    }
                }
                if (reponse_unique != 1) //-----si elle est pas exactement  a 1 , notre variable prend 1 ce qui signifie que il y'a au moins une erreure dans le questionnaire
                    fichier_non_eligible = 1;

                break;
            case '*':          //------------------------------- "*" signifie fin de question bivalente et debut de bareme de la question bivalente
                nbr_qst_biv++; //--------- incremente a chaque fois que une question de type bivalent est trouvable dans le fichier
                caractere = fgetc(fichier);
                while (caractere != '*') //-----------le caractere delimite aussi le bloc ou se trouve le bareme de la question
                {
                    caractere = fgetc(fichier);
                    if (caractere == '|') //------------------ et on le refait la même chose  que pour le choix unique mais cette fois sur bivalent
                    {
                        fscanf(fichier, "%d", &resultat);
                        if (resultat > 0)
                        {
                            *somme += resultat; //---- si le bareme de la note est positif on l'ajoute a somme qui correspond a la note totale du questionnaire
                            reponse_bivalent++; //--------- l'incrementation ne doit pas doubler 1
                        }
                    }
                    else if (caractere == ',')
                    {
                        fscanf(fichier, "%d", &resultat);
                        if (resultat > 0)
                        {
                            *somme += resultat;
                            reponse_bivalent++;
                        }
                    }
                }
                if (reponse_bivalent != 1)
                    fichier_non_eligible = 1;

                break;
            case '^':          //------------------------------- "^" signifie fin de question multiple et debut de bareme de la question multiple
                nbr_qst_mul++; //--------- incremente a chaque fois que une question de type multiple est trouvable dans le fichier
                caractere = fgetc(fichier);
                while (caractere != '^') //-----------le caractere delimite aussi le bloc ou se trouve le bareme de la question
                {
                    caractere = fgetc(fichier);
                    if (caractere == '|') //------------------ et on le refait la même chose que pour le choix unique c'est a dire  que ce char signifie debut note du choix 1
                    {
                        fscanf(fichier, "%d", &resultat);
                        if (resultat > 0)
                        {
                            *somme += resultat;
                            reponse_multiple++; //------------- l'incrementation doit être >= 2 au minimum
                        }
                    }
                    else if (caractere == ',')
                    {
                        fscanf(fichier, "%d", &resultat);
                        if (resultat > 0)
                        {
                            *somme += resultat;
                            reponse_multiple++;
                        }
                    }
                    else if (caractere == '.')
                    {
                        fscanf(fichier, "%d", &resultat);
                        if (resultat > 0)
                        {
                            *somme += resultat;
                            reponse_multiple++;
                        }
                    }
                }
                if (reponse_multiple < 2)
                    fichier_non_eligible = 1;
                break;
            }
            reponse_bivalent = 0; //---------- on remet le compteur a 0 car on doit lire tout le fichier , plusieurs question bivalente unique peuvent s'y trouver
            reponse_multiple = 0;
            reponse_unique = 0;

        } while (caractere != EOF); //--------- on lit tout les char different de EOF ( fin de fichier )

        if ((fichier_non_eligible == 0 && nbr_qst_biv >= 1) && (nbr_qst_mul >= 1 && nbr_qst_uniq >= 1)) //----- si y'a jamais eu d'erreur c'est bon ! sinon erreur
            return 1;
        else
            return 0;

        fclose(fichier); //----------- fermeture fichier
    }
    else
    {
        printf("Erreur de verification du fichier ! ");
        return -1;
    }
}
//---- MERCI ! Suffit de build and run pour tester ! :)

/*
    Il faut savoir que dans mon fichier j'ai mis des caracteres speciaux specifiques pour savoir Quand est ce que on lit une question de Type Bivalent/Unique/Multiple
    Mais aussi Quand est ce que on lit un bareme de question de type Bivalent/unique/Multiple ce qui permet de se retrouver + facilement :

    Fin Question unique et Debut bareme unique  - > caractere '~'     et un 2eme '~' est placé juste apres pour marqué la fin du bareme unique
    Fin Question multiple et Debut bareme multiple  -> caractere '^'   et un 2eme '^' est placé juste apres pour marqué la fin du bareme multiple
    Fin Question bivalent  et Debut bareme bivalent  -> caractere '*'    et un 2eme '*' est placé juste apres pour marqué la fin du bareme bivalent

    Bareme choix 1 -> caracetere '|'
    Bareme choix 2 -> caracetere ','
    Bareme choix 3 -> caracetere '.'

    Voila merci !

*/
