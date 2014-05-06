/*
 *   Ce fichier fait parti d'un projet de programmation donné en Licence 3 
 *   à l'Université de Bordeaux 1
 *
 *   Copyright (C) 2014 Adrien Boussicault
 *
 *    This Library is free software: you can redistribute it and/or modify
 *    it under the terms of the GNU General Public License as published by
 *    the Free Software Foundation, either version 2 of the License, or
 *    (at your option) any later version.
 *
 *    This Library is distributed in the hope that it will be useful,
 *    but WITHOUT ANY WARRANTY; without even the implied warranty of
 *    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *    GNU General Public License for more details.
 *
 *    You should have received a copy of the GNU General Public License
 *    along with this Library.  If not, see <http://www.gnu.org/licenses/>.
 */


#ifndef __AUTOMATE_H__
#define __AUTOMATE_H__

#include "ensemble.h"

/**
 * \brief Le type d'un automate.
 * 
 * Ce type code un automate. Cette automate peut être non deterministe, ses 
 * états sont des entiers codés par le 
 * type int. Les lettres sont codées par le type char, et l'automate n'accepte 
 * pas d'epsilon transition.
 * L'automate codé, peut avoir plusieurs états initiaux.
 * 
 */
typedef struct _Automate Automate;

/**
 * \brief Créer un automate vide, sans états, sans lettres et sans transitions.
 *
 * \return L'automate créé.
 */
Automate * creer_automate();

/**
 * \brief Détruit un automate.
 * 
 * \param automate L'automate à détruire.
 */ 
void liberer_automate( Automate * automate);

/**
 * \brief Ajoute un état à un automate passé en paramètre.
 *
 * \param automate Un automate
 * \param etat l'état à ajouter
 */ 
void ajouter_etat( Automate * automate, int etat );

/**
 * \brief Ajoute une lettre à un automate passé en paramètre.
 *
 * \param automate Un automate
 * \param lettre La lettre à ajouter
 */ 
void ajouter_lettre( Automate * automate, char lettre );

/**
 * \brief Ajoute une transition à l'automate passé en paramètre.
 *
 * Si les états et la lettre de la transition n'existent pas dans l'automate,
 * alors, la lettre et les états sont ajoutés automatiquement à l'automate.
 *
 * \param automate Un automate
 * \param origine L'origine de la transition
 * \param lettre La lettre de la transition
 * \param La fin de la transition
 */ 
void ajouter_transition(
	Automate * automate, int origine, char lettre, int fin
);

/**
 * \brief Ajoute un état final à un automate passé en paramètre.
 *
 * Si l'état n'existe pas dans l'automate, l'état est ajouté 
 * automatiquement à l'automate.
 *
 * \param automate Un automate
 * \origine etat_final L'état final
 */ 
void ajouter_etat_final(
	Automate * automate, int etat_final
);

/**
 * \brief Ajoute un état initial à un automate passé en paramètre.
 *
 * Si l'état n'existe pas dans l'automate, l'état est ajouté 
 * automatiquement à l'automate.
 *
 * \param automate Un automate
 * \origine etat_initial L'état initial
 */ 
void ajouter_etat_initial(
	Automate * automate, int etat_initial
);

/**
 * \brief Renvoie l'ensemble des états d'un automate
 *
 * La mémoire de l'ensemble renvoyé est gérée par l'automate.
 * L'utilisateur ne doit donc pas modifier ou libérer l'ensemble ainsi obtenu.
 *
 * \param automate Un automate
 * \return L'ensmble des états de l'automate
 */ 
const Ensemble * get_etats( const Automate* automate );

/**
 * \brief Renvoie l'ensemble des états intiaux d'un automate
 *
 * La mémoire de l'ensemble renvoyé est gérée par l'automate.
 * L'utilisateur ne doit donc pas modifier ou libérer l'ensemble ainsi obtenu.
 *
 * \param automate Un automate
 * \return L'ensmble des états initiaux de l'automate
 */ 
const Ensemble * get_initiaux( const Automate* automate );

/**
 * \brief Renvoie l'ensemble des états finaux d'un automate
 *
 * La mémoire de l'ensemble renvoyé est gérée par l'automate.
 * L'utilisateur ne doit donc pas modifier ou libérer l'ensemble ainsi obtenu.
 *
 * \param automate Un automate
 * \return L'ensmble des états finaux de l'automate
 */ 
const Ensemble * get_finaux( const Automate* automate );

/**
 * \brief Renvoie l'alphabet d'un automate
 *
 * La mémoire de l'ensemble renvoyé est gérée par l'automate.
 *
 * \param automate Un automate
 * \return L'ensmble des lettres de l'automate
 */ 
const Ensemble * get_alphabet( const Automate* automate );

/**
 * \brief Renvoie 1 si ('origine', 'lettre', 'fin') est une transition de 
 *        l'automate et 0 sinon.
 *
 * \param automate Un automate
 * \param origine L'origine de la transition
 * \param lettre La lettre de la transition
 * \param fin La fin de la transition
 * \return 1 ou 0
 */ 
int est_une_transition_de_l_automate(
	const Automate* automate,
	int origine, char lettre, int fin
);

/**
 * \brief Renvoie 1 si ('etat') est un état de l'automate et 0 sinon.
 *
 * \param automate Un automate
 * \param etat Un état
 * \return 1 ou 0
 */ 
int est_un_etat_de_l_automate( const Automate* automate, int etat );

/**
 * \brief Renvoie 1 si ('etat') est un état initial de l'automate et 0 sinon.
 *
 * \param automate Un automate
 * \param etat Un état
 * \return 1 ou 0
 */ 
int est_un_etat_initial_de_l_automate( const Automate* automate, int etat );

/**
 * \brief Renvoie 1 si ('etat') est un état final initial de l'automate et 0 
 *        sinon.
 *
 * \param automate Un automate
 * \param etat Un état
 * \return 1 ou 0
 */ 
int est_un_etat_final_de_l_automate( const Automate* automate, int etat );

/**
 * \brief Renvoie 1 si ('etat') est une lettre de  l'automate et 0 sinon.
 *
 * \param automate Un automate
 * \param lettre Une lettre
 * \return 1 ou 0
 */ 
int est_une_lettre_de_l_automate( const Automate* automate, char lettre );

/**
 * \brief Renvoie l'ensemble des états accessibles à partir d'un état donné
 *        en paramètre et en lisant une lettre donné en paramètre.
 *
 * La mémoire de l'ensemble renvoyé par la fonction est laissée à la charge de 
 * l'utilisateur. L'utilisateur devra donc prendre soin de libérer la mémoire
 * à la fin de son utilisation.
 *
 * \param automate Un automate
 * \param origine Un état
 * \param lettre Une lettre
 * \return L'ensemble des états accessibles.
 */ 
Ensemble * delta1(
	const Automate* automate, int origine, char lettre
);

/**
 * \brief Renvoie l'ensemble des états accéssibles à partir d'un ensemble 
 *        d'états donné en paramètre et en lisant une lettre donné en 
 *        paramètre.
 *
 * La mémoire de l'ensemble renvoyé par la fonction est laissée à la charge de 
 * l'utilisateur. L'utilisateur devra donc prendre soin de libérer la mémoire
 * à la fin de son utilisation.
 *
 * \param automate Un automate
 * \param etats_courants L'ensemble des état origines
 * \param lettre Une lettre
 * \return L'ensemble des états accessibles.
 */ 
Ensemble * delta(
	const Automate* automate, const Ensemble * etats_courants, char lettre
);

/**
 * \brief Renvoie l'ensemble des états accessibles à partir d'un ensemble 
 *        d'états donné en paramètre et en lisant un mot donné en 
 *        paramètre.
 *
 * La mémoire de l'ensemble renvoyé par la fonction est laissé à la charge de 
 * l'utilisateur. L'utilisateur devra donc prendre soin de libérer la mémoire
 * à la fin de son utilisation.
 *
 * \param automate Un automate
 * \param etats_courants L'ensemble des état origines
 * \param mot Le mot à lire
 * \return L'ensemble des états accessibles.
 */ 
Ensemble * delta_star(
	const Automate* automate, const Ensemble * etats_courants, const char* mot
);

/**
 * \brief Renvoie vrai si le mot passé en paramètre est reconu par l'automate 
 *        passé en paramètre, et renvoie 0 sinon.
 *
 * \param automate Un automate
 * \param mot Le mot à reconaître
 * \return 1 ou 0
 */ 
int le_mot_est_reconnu( const Automate* automate, const char* mot );

/**
 * \brief La fonction passe en revue toutes les transitions de l'automate et 
 *        appelle la fonction passé en paramtre.
 *
 * La fonction qui sera executéei (et qui a été passé en paramètre), doit 
 * posséder l'entête suivante :
 *   void NOM_FONCTION( int origine, char lettre, int fin, void* data );
 * Les pramètres 'origine', 'lettre' et 'fin' correspondent à l'origine, la 
 * lettre et la fin de la transitions en cours de parcours.
 * Le paramètre 'data' est un pointeur qui sera identique à celui passé par le
  paramètre 'data' de la fonction pour_toute_transition().
 *
 * \param automate Un automate
 * \param action La fonction à exécuter
 * \param data La donnée supplémentaire à passer en paramètre à la fonction 
 *             'action' executée à chaque transition.
 */ 
void pour_toute_transition(
	const Automate* automate,
	void (* action )( int origine, char lettre, int fin, void* data ),
	void* data
);


/**
 * \brief Copie un automate.
 *
 * L'automate copié et l'automate à copier sont indépendants du point de vue de 
 * la mémoire.
 *
 * \param automate L'automate à copier
 * \return La copie de l'automate
 */ 
Automate* copier_automate( const Automate* automate );

/**
 * \brief Renvoie un automate qui reconnait un unique mot passé en paramètre.
 *
 * \param mot le mot à reconnaître
 * \return Un automate
 */ 
Automate * mot_to_automate( const char * mot );

/**
 * \brief Renvoie l'ensemble des états accessibles à partir d'un état en lisant 
 *        un mot quelcquonque.
 *
 * \param automate Un automate
 * \param etat L'état de départ
 * \return L'ensemble des états accessibles
 */ 
Ensemble* etats_accessibles( const Automate * automate, int etat );

/**
 * \brief Renvoie l'automate passé en paramètre dont les états non accessible 
 *        ont été supprimés.
 *
 * \param automate Un automate
 * \return L'automate accessible
 */ 
Automate *automate_accessible( const Automate * automate);

/**
 * \brief Renvoie l'automate miroir d'un automate.
 *
 * Il s'agit de l'automate qui reconnait les mots renversés du langage associé
 * à l'automate passé en paramètre.
 *
 * \param automate Un automate
 * \return L'automate miroir.
 */ 
Automate *miroir( const Automate * automate);

/**
 * \brief Renvoie l'automate passé en paramètre dont les états non 
 *        co-accessibles ont été supprimés.
 *
 * Un état est co-accessible si il existe un chemin dans l'automate qui relie 
 * cet état à un état terminal.
 *
 * \param automate Un automate
 * \return L'automate co-accessibe
 */ 
Automate *automate_co_accessible( const Automate * automate);

/**
 * \brief Créer l'automate des préfixes d'un automate.
 *
 * Cet automate reconait tous les préfixes des mots du langage reconu par l'automate
 * passé en paramètre.
 *
 * \param automate L'automate de départ
 * \return L'automate à créer
 */ 
Automate * creer_automate_des_prefixes( const Automate* automate );

/**
 * \brief Créer l'automate des suffixes d'un automate.
 *
 * Cet automate reconait tous les suffixes des mots du langage reconu par l'automate
 * passé en paramètre.
 *
 * \param automate L'automate de départ
 * \return L'automate à créer
 */ 
Automate * creer_automate_des_suffixes( const Automate* automate );

/**
 * \brief Créer l'automate des facteurs à un automate.
 *
 * Cet automate reconait tous les suffixes d'un langage reconu par un automate
 * passé en paramètre.
 *
 * \param automate L'automate de départ
 * \return L'automate à créer
 */
Automate * creer_automate_des_facteurs( const Automate* automate );


/**
 * \brief Créer un automate qui reconnaît tous les mots possédant pour sous mot
 *        au moins un des mots reconnu par un autre automate donné en paramètre.
 *
 * \param automate L'automate de départ
 * \param alphabet Un alphabet contenant des lettres additionelles à ajouter.
 * \return L'automate à créer
 */ 
Automate * creer_automate_des_sur_mot(
	const Automate* automate, Ensemble * alphabet
);

/**
 * \brief Créer l'automate d'une concaténation.
 *
 * Cet automate reconait tous les mots qui sont concaténation d'un mot du
 * langage du premier automate passé en paramètre, et d'un mot du
 * langage du second automate passé en paramètre.
 *
 * \param automate L'automate de départ
 * \return L'automate à créer
 */ 
Automate * creer_automate_de_concatenation( const Automate* automate1, const Automate* automate2 );


/**
 * \brief Créer l'automate des sous-mots à un automate.
 *
 * Cet automate reconait touts les sous-mots d'un langage reconu par un automate
 * passé en paramètre.
 *
 * \param automate L'automate de départ
 * \return L'automate à créer
 */ 
Automate * creer_automate_des_sous_mots( const Automate* automate );


/**
  * \brief Créer l'automate du mélange.
  * 
  * Créer un nouvel automate qui reconaît les mots w tel que w est le mélange de
  * deux mots w1 et w2 appartenant respectivement aux langages reconnus par
  * l'automate 1 et par l'autoamte 2 passés en paramètre.
  *
  * On peut dfinir le mélange récursivement par :
  *     melange( a.w1, b.W2 ) := a.melange( w1, b.w2 ) + b.melange( a.w1, w2 )
  *     melange( w, epsilon ) := w
  *     melange( epsilon, w ) := w
  * où w1, w2 et w3 sont des mots, a et b des lettres, epsilon l'epsilon 
  * transition et . la concaténation.
  *
  * \param automate1 Le premier automate
  * \param automate2 Le deuième automate
  * \return L'auomtate du mélange
  */
Automate * creer_automate_du_melange( const Automate* automate1,  const Automate* automate2 );

/**
 * \brief Affiche sur l'entrée standard (stdout) l'automate passé en paramètre
 *
 * \param automate L'automate à afficher
 */ 
void print_automate( const Automate * automate );

#endif
