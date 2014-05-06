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


#include "automate.h"
#include "outils.h"
#include "fifo.h"

#include <signal.h>
#include <errno.h>

#define BEGIN_TEST printf("\n================================================================================\nTest de %s() ...\n================================================================================\n", __FUNCTION__);

Fifo* fifo = NULL;
int nb_total_test = 0;
int nb_test = 0;

typedef struct {
	int (*test)();
} Test;

Test* creer_test( int (*test)() ){
	Test* res = xmalloc( sizeof(Test) );
	res->test = test;
	return res;
}

void liberer_test( Test* test ){
	xfree( test );
}

void start_or_continue_test(){
	while( ! est_vide(fifo) ){
		Test* data = (Test*) retirer_fifo( fifo );
		int (*test)();
		test = data->test;
		liberer_test( data );
		nb_total_test += 1;
		int res = test();
		printf("================================================================================\n");
		if(res){
			printf("... \033[32mvalidé.\033[0m\n");
		}else{
			printf("... \033[31mECHEC !\033[0m\n");
		}
		printf("================================================================================\n\n");
		nb_test += res;
	}
	liberer_fifo( fifo );
	printf( "\n" );
	printf( "\n Nb de tests effectués : %d", nb_total_test);
	printf( "\n Nb de tests validés : \033[32m%d\033[0m", nb_test);
	printf( "\n Nb de tests ratés : \033[31m%d\033[0m", nb_total_test - nb_test);
	printf( "\n" );
	exit( nb_test );
}

void set_all_sigactions();

void execute_on_sigfault( int signal ){
	printf("================================================================================\n");
	printf("... \033[31mECHEC ! Le test a fait une erreur de segmentation.\033[0m\n");
	printf("================================================================================\n\n");

	set_all_sigactions();

	start_or_continue_test();
}

void ajouter_test( int (*test)() ){
	ajouter_fifo( fifo, (intptr_t) creer_test( test ) );
}


void set_sigaction( int signum, void (*execute_on_signal)( int signal ) ){

	struct sigaction action;

	int res = sigaction( signum, NULL, &action );

	if( res != 0 ){
		fprintf(stderr, "Erreur pour installer le signal : %d \n" , errno );
	}    

	if( action.sa_handler != SIG_IGN ){
		action.sa_handler = execute_on_signal;
		action.sa_flags =  SA_RESTART | SA_NODEFER;
		sigemptyset (&action.sa_mask);
		sigaction(signum, &action, NULL);
	}
}

void set_all_sigactions(){

	set_sigaction( SIGILL, execute_on_sigfault );
	set_sigaction( SIGFPE, execute_on_sigfault );
	set_sigaction( SIGSEGV, execute_on_sigfault );
	set_sigaction( SIGBUS, execute_on_sigfault );
	set_sigaction( SIGTRAP, execute_on_sigfault );

}

int test_creer_automate(){
	BEGIN_TEST;

	int result = 1;

	Automate * automate = creer_automate();

	ajouter_lettre( automate, 'a' );
	ajouter_lettre( automate, 'd' );

	ajouter_etat( automate, 3 );
	ajouter_etat_final( automate, 6 );

	ajouter_transition( automate, 3, 'a', 5 );
	ajouter_transition( automate, 3, 'b', 3 );

	ajouter_etat_final( automate, 5 );
	ajouter_etat_initial( automate, 3 );

	TEST( 
		1
		&& est_un_etat_de_l_automate( automate, 3)
		&& est_un_etat_de_l_automate( automate, 5)
		&& est_un_etat_de_l_automate( automate, 6)
		, result
	);
	TEST( 
		1
		&& ! est_un_etat_final_de_l_automate( automate, 3)
		&& est_un_etat_final_de_l_automate( automate, 5)
		&& est_un_etat_final_de_l_automate( automate, 6)
		, result
	);
	TEST( 
		1
		&& est_un_etat_initial_de_l_automate( automate, 3)
		&& ! est_un_etat_initial_de_l_automate( automate, 5)
		, result
	);
	TEST( 
		1
		&& est_une_lettre_de_l_automate( automate, 'a')
		&& est_une_lettre_de_l_automate( automate, 'b')
		&& est_une_lettre_de_l_automate( automate, 'd')
		&& ! est_une_lettre_de_l_automate( automate, 'c')
		, result
	);
	TEST( 
		1
		&& est_une_transition_de_l_automate( automate, 3, 'a', 5 )
		&& est_une_transition_de_l_automate( automate, 3, 'b', 3 )
		&& ! est_une_transition_de_l_automate( automate, 3, 'b', 5 )
		&& ! est_une_transition_de_l_automate( automate, 3, 'a', 3 )
		&& ! est_une_transition_de_l_automate( automate, 5, 'a', 3 )
		&& ! est_une_transition_de_l_automate( automate, 5, 'b', 3 )
		&& ! est_une_transition_de_l_automate( automate, 5, 'a', 5 )
		&& ! est_une_transition_de_l_automate( automate, 5, 'b', 5 )
		, result
	);

	const Ensemble* ens = get_initiaux( automate );

	TEST( 
		1
		&& ens
		&& est_dans_l_ensemble( ens, 3 )
		&& ! est_dans_l_ensemble( ens, 5 )
		&& ! est_dans_l_ensemble( ens, 6 )
		, result
	);

	ens = get_finaux( automate );

	TEST( 
		1
		&& ens
		&& ! est_dans_l_ensemble( ens, 3 )
		&& est_dans_l_ensemble( ens, 5 )
		&& est_dans_l_ensemble( ens, 6 )
		, result
	);

	ens = get_alphabet( automate );

	TEST( 
		1
		&& ens
		&& est_dans_l_ensemble( ens, 'a')
		&& est_dans_l_ensemble( ens, 'b')
		&& ! est_dans_l_ensemble( ens, 'c')
		&& est_dans_l_ensemble( ens, 'd')
		, result
	);


	liberer_automate( automate );

	return result;
}


int test_delta_delta_star(){
	BEGIN_TEST;

	int result = 1;

	Automate* automate = creer_automate();


	ajouter_etat( automate, 3 );
	ajouter_etat( automate, 5 );
	ajouter_transition( automate, 3, 'a', 5 );
	ajouter_transition( automate, 5, 'b', 3 );
	ajouter_transition( automate, 5, 'a', 5 );
	ajouter_transition( automate, 5, 'c', 6 );
	ajouter_etat_initial( automate, 3 );
	ajouter_etat_final( automate, 6 );

	TEST(
		1
		&& est_un_etat_de_l_automate( automate, 3 )
		&& est_un_etat_de_l_automate( automate, 5 )
		&& est_un_etat_de_l_automate( automate, 6 )

		&& est_un_etat_initial_de_l_automate( automate, 3 )
		&& ! est_un_etat_initial_de_l_automate( automate, 5 )
		&& ! est_un_etat_initial_de_l_automate( automate, 6 )

		&& ! est_un_etat_final_de_l_automate( automate, 3 )
		&& ! est_un_etat_final_de_l_automate( automate, 5 )
		&& est_un_etat_final_de_l_automate( automate, 6 )
		
		&& est_une_lettre_de_l_automate( automate, 'a' )
		&& est_une_lettre_de_l_automate( automate, 'b' )
		&& est_une_lettre_de_l_automate( automate, 'c' )
		&& ! est_une_lettre_de_l_automate( automate, 'd' )

		&& est_une_transition_de_l_automate( automate, 3, 'a', 5 )
		&& est_une_transition_de_l_automate( automate, 5, 'b', 3 )
		&& est_une_transition_de_l_automate( automate, 5, 'a', 5 )
		&& est_une_transition_de_l_automate( automate, 5, 'c', 6 )
		
		&& ! est_une_transition_de_l_automate( automate, 3, 'b', 5 )
		&& ! est_une_transition_de_l_automate( automate, 1, 'b', 5 )
		&& ! est_une_transition_de_l_automate( automate, 3, 'a', 1 )
		, result
	);

	Ensemble * etat_courant = creer_ensemble( NULL, NULL, NULL );
	ajouter_element( etat_courant, 3 ); 

	deplacer_ensemble( etat_courant, delta( automate, etat_courant, 'a' ) );

	TEST(
		1
		&& est_dans_l_ensemble( etat_courant, 5 )
		&& taille_ensemble( etat_courant ) ==1
		, result
	);

	deplacer_ensemble(
		etat_courant, delta_star( automate, etat_courant, "ab" ) 
	);

	TEST(
		1
		&& est_dans_l_ensemble( etat_courant, 3 )
		&& taille_ensemble( etat_courant ) ==1
		, result
	);

	deplacer_ensemble(
		etat_courant, delta_star( automate, etat_courant, "ac" )
	);

	TEST(
		1
		&& est_dans_l_ensemble( etat_courant, 6 )
		&& taille_ensemble( etat_courant ) ==1
		, result
	);

	deplacer_ensemble(
		etat_courant, delta_star( automate, etat_courant, "" )
	);

	TEST(
		1
		&& est_dans_l_ensemble( etat_courant, 6 )
		&& taille_ensemble( etat_courant ) ==1
		, result
	);

	deplacer_ensemble(
		etat_courant, delta_star( automate, etat_courant, "c" )
	);

	TEST(
		1
		&& taille_ensemble( etat_courant ) == 0
		, result
	);

	liberer_ensemble( etat_courant );
	liberer_automate( automate );

	return result;
}


int test_execute_fonctions(){
	BEGIN_TEST

	int res = 1;
	
	Automate * automate2;
	Automate * automate;
	Automate * result = NULL;
	Ensemble * ens = NULL;
	Ensemble * ens1 = NULL;
	
	automate  = creer_automate();
	result = NULL;
		ajouter_lettre( automate, 'a' );
	if( result ) liberer_automate( result );
	liberer_automate( automate );

	automate  = creer_automate();
	result = NULL;
		ajouter_etat_final( automate, 1 );
	if( result ) liberer_automate( result );
	liberer_automate( automate );

	automate  = creer_automate();
	result = NULL;
		ajouter_etat_initial( automate, 1 );
	if( result ) liberer_automate( result );
	liberer_automate( automate );

	automate  = creer_automate();
	get_initiaux( automate );
	liberer_automate( automate );

	automate  = creer_automate();
	get_finaux( automate );
	liberer_automate( automate );

	automate  = creer_automate();
	get_alphabet( automate );
	liberer_automate( automate );

	automate  = creer_automate();
	result = NULL;
	est_un_etat_initial_de_l_automate( automate, 1 );
	if( result ) liberer_automate( result );
	liberer_automate( automate );

	automate  = creer_automate();
	result = NULL;
	est_un_etat_final_de_l_automate( automate, 1 );
	if( result ) liberer_automate( result );
	liberer_automate( automate );

	automate  = creer_automate();
	result = NULL;
	est_une_lettre_de_l_automate( automate, 'a' );
	if( result ) liberer_automate( result );
	liberer_automate( automate );

	automate  = creer_automate();
	ens = NULL;
	ens1 = creer_ensemble( NULL, NULL, NULL );
	ens =  delta_star( automate, ens1, "aba" );
	if( ens ) liberer_ensemble( ens );
	liberer_ensemble( ens1 );
	liberer_automate( automate );

	automate  = creer_automate();
	result = NULL;
	le_mot_est_reconnu( automate, "abaa" );
	if( result ) liberer_automate( result );
	liberer_automate( automate );

	automate  = creer_automate();
	result = NULL;
		result =  mot_to_automate( "abbaa" );
	if( result ) liberer_automate( result );
	liberer_automate( automate );

	automate  = creer_automate();
	ens = NULL;
	ens = etats_accessibles( automate, 0 );
	if( ens ) liberer_ensemble( ens );
	liberer_automate( automate );

	automate  = creer_automate();
	result = NULL;
	result = automate_accessible( automate );
	if( result ) liberer_automate( result );
	liberer_automate( automate );

	automate  = creer_automate();
	result = NULL;
	result = miroir( automate );
	if( result ) liberer_automate( result );
	liberer_automate( automate );

	automate  = creer_automate();
	result = NULL;
	result = automate_co_accessible(automate );
	if( result ) liberer_automate( result );
	liberer_automate( automate );

	automate  = creer_automate();
	result = NULL;
	result = creer_automate_des_prefixes( automate );
	if( result ) liberer_automate( result );
	liberer_automate( automate );

	automate  = creer_automate();
	result = NULL;
	result = creer_automate_des_suffixes( automate );
	if( result ) liberer_automate( result );
	liberer_automate( automate );

	automate  = creer_automate();
	result = NULL;
	result = creer_automate_des_facteurs( automate );
	if( result ) liberer_automate( result );
	liberer_automate( automate );

	automate  = creer_automate();
	result = NULL;
	ens = creer_ensemble( NULL, NULL, NULL );
	result =  creer_automate_des_sur_mot(
		automate, ens
	);
	if( result ) liberer_automate( result );
	liberer_automate( automate );
	liberer_ensemble( ens );

	automate  = creer_automate();
	automate2  = creer_automate();
	result = NULL;
	result = creer_automate_de_concatenation( automate, automate2 );
	if( result ) liberer_automate( result );
	liberer_automate( automate );
	liberer_automate( automate2 );

	automate  = creer_automate();
	result = NULL;
	result = creer_automate_des_sous_mots( automate );
	if( result ) liberer_automate( result );
	liberer_automate( automate );

	return res;
}

int test_mot_accepte(){
	BEGIN_TEST;

	int result = 1;

	Automate * automate = creer_automate();

	ajouter_transition( automate, 1, 'a', 1 );
	ajouter_transition( automate, 1, 'b', 2 );

	ajouter_etat_final( automate, 2 );
	ajouter_etat_initial( automate, 1 );

	TEST( le_mot_est_reconnu( automate, "b" ), result );
	TEST( le_mot_est_reconnu( automate, "ab" ), result );
	TEST( le_mot_est_reconnu( automate, "aab" ), result );
	TEST( ! le_mot_est_reconnu( automate, "aba" ), result );

	liberer_automate( automate );

	return result;

}

int test_automate_vide(){
	BEGIN_TEST;

	int result = 1;

	Automate * automate = creer_automate();

	TEST( 
		1
		&& ! est_un_etat_de_l_automate( automate, 0)
		&& ! est_un_etat_final_de_l_automate( automate, 0)
		&& ! est_un_etat_initial_de_l_automate( automate, 0)
		&& ! est_une_lettre_de_l_automate( automate, 'a')
		&& ! est_une_transition_de_l_automate( automate, 0, 'a', 0 )
		, result
	);

	liberer_automate( automate );

	return result;

}

int main(){
	nb_test = 0;
	nb_total_test = 0;
	fifo = creer_fifo();

	ajouter_test( test_execute_fonctions );
	ajouter_test( test_delta_delta_star );
	ajouter_test( test_creer_automate );
	ajouter_test( test_mot_accepte );
	ajouter_test( test_automate_vide );

	set_all_sigactions();
	
	start_or_continue_test();

	exit( nb_test );
}
