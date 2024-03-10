/**
* Programme qui reprend le tp2 et qui apporte de nombreuses modifications à celles-ci, dont l'usage de pointeurs intelligents et de templates.
* \file   tp3.cpp
* \author Song Ning Lan et Sheng He Ge
* \date   25 février 2024
* Créé le 29 janvier 2024
*/
#pragma region "Includes"//{
#define _CRT_SECURE_NO_WARNINGS // On permet d'utiliser les fonctions de copies de chaînes qui sont considérées non sécuritaires.

#include "structures.hpp"      // Structures de données pour la collection de films en mémoire.

#include <iostream>
#include <fstream>
#include <string>
#include <limits>
#include <algorithm>
#include <sstream>
#include <vector>
#include <iomanip>
#include <variant>

#include "cppitertools/range.hpp"
#include "gsl/span"

#include "bibliotheque_cours.hpp"
#include "verification_allocation.hpp" // Nos fonctions pour le rapport de fuites de mémoire.
#include "debogage_memoire.hpp"        // Ajout des numéros de ligne des "new" dans le rapport de fuites.  Doit être après les include du système, qui peuvent utiliser des "placement new" (non supporté par notre ajout de numéros de lignes).

using namespace std;
using namespace iter;
using namespace gsl;

#pragma endregion//}

typedef uint8_t UInt8;
typedef uint16_t UInt16;

#pragma region "Fonctions de base pour lire le fichier binaire"//{
template <typename T>
T lireType(istream& fichier)
{
	T valeur{};
	fichier.read(reinterpret_cast<char*>(&valeur), sizeof(valeur));
	return valeur;
}
#define erreurFataleAssert(message) assert(false&&(message)),terminate()
static const uint8_t enteteTailleVariableDeBase = 0xA0;
size_t lireUintTailleVariable(istream& fichier)
{
	uint8_t entete = lireType<uint8_t>(fichier);
	switch (entete) {
	case enteteTailleVariableDeBase + 0: return lireType<uint8_t>(fichier);
	case enteteTailleVariableDeBase + 1: return lireType<uint16_t>(fichier);
	case enteteTailleVariableDeBase + 2: return lireType<uint32_t>(fichier);
	default:
		erreurFataleAssert("Tentative de lire un entier de taille variable alors que le fichier contient autre chose à cet emplacement.");
	}
}

string lireString(istream& fichier)
{
	string texte;
	texte.resize(lireUintTailleVariable(fichier));
	fichier.read((char*)&texte[0], streamsize(sizeof(texte[0])) * texte.length());
	return texte;
}

#pragma endregion//}

//TODO: Une fonction pour ajouter un Film à une ListeFilms, le film existant déjà; on veut uniquement ajouter le pointeur vers le film existant.  Cette fonction doit doubler la taille du tableau alloué, avec au minimum un élément, dans le cas où la capacité est insuffisante pour ajouter l'élément.  Il faut alors allouer un nouveau tableau plus grand, copier ce qu'il y avait dans l'ancien, et éliminer l'ancien trop petit.  Cette fonction ne doit copier aucun Film ni Acteur, elle doit copier uniquement des pointeurs.

void ListeFilms::changeDimension(int nouvelleCapacite)
{
	Film** nouvelleListe = new Film * [nouvelleCapacite];

	if (elements_ != nullptr) {  // Noter que ce test n'est pas nécessaire puique nElements sera zéro si elements est nul, donc la boucle ne tentera pas de faire de copie, et on a le droit de faire delete sur un pointeur nul (ça ne fait rien).
		nElements_ = min(nouvelleCapacite, nElements_);
		for (int i : range(nElements_))
			nouvelleListe[i] = elements_[i];
		delete[] elements_;
	}

	elements_ = nouvelleListe;
	capacite_ = nouvelleCapacite;
}

void ListeFilms::ajouterFilm(Film* film)
{
	if (nElements_ == capacite_)
		changeDimension(max(1, capacite_ * 2));
	elements_[nElements_++] = film;
}


//TODO: Une fonction pour enlever un Film d'une ListeFilms (enlever le pointeur) sans effacer le film; la fonction prenant en paramètre un pointeur vers le film à enlever.  L'ordre des films dans la liste n'a pas à être conservé.
// On a juste fait une version const qui retourne un span non const.  C'est valide puisque c'est la struct qui est const et non ce qu'elle pointe.  Ça ne va peut-être pas bien dans l'idée qu'on ne devrait pas pouvoir modifier une liste const, mais il y aurais alors plusieurs fonctions à écrire en version const et non-const pour que ça fonctionne bien, et ce n'est pas le but du TD (il n'a pas encore vraiment de manière propre en C++ de définir les deux d'un coup).
span<Film*> ListeFilms::enSpan() const { return span(elements_, nElements_); }

void ListeFilms::enleverFilm(const Film* film)
{
	for (Film*& element : enSpan()) {  // Doit être une référence au pointeur pour pouvoir le modifier.
		if (element == film) {
			if (nElements_ > 1)
				element = elements_[nElements_ - 1];
			nElements_--;
			return;
		}
	}
}

//TODO: Une fonction pour trouver un Acteur par son nom dans une ListeFilms, qui retourne un pointeur vers l'acteur, ou nullptr si l'acteur n'est pas trouvé.  Devrait utiliser span.
// Voir la NOTE ci-dessous pourquoi Acteur* n'est pas const.  Noter que c'est valide puisque c'est la struct uniquement qui est const dans le paramètre, et non ce qui est pointé par la struct.
//span<Acteur*> spanListeActeurs(const ListeActeurs& liste) { return span(liste.elements, liste.nElements); }

//NOTE: Doit retourner un Acteur modifiable, sinon on ne peut pas l'utiliser pour modifier l'acteur tel que demandé dans le main, et on ne veut pas faire écrire deux versions.
shared_ptr<Acteur> ListeFilms::trouverActeur(const string& nomActeur) const
{
	for (const Film* film : enSpan()) {
		for (const shared_ptr<Acteur>& acteur : film->acteurs.obtenirElements()) {
			if (acteur->nom == nomActeur)
				return acteur;
		}
	}
	return nullptr;
}

//TODO: Compléter les fonctions pour lire le fichier et créer/allouer une ListeFilms.  La ListeFilms devra être passée entre les fonctions, pour vérifier l'existence d'un Acteur avant de l'allouer à nouveau (cherché par nom en utilisant la fonction ci-dessus).
shared_ptr<Acteur> lireActeur(istream& fichier, ListeFilms& listeFilms)
{
	Acteur acteur = {};
	acteur.nom = lireString(fichier);
	acteur.anneeNaissance = int(lireUintTailleVariable(fichier));
	acteur.sexe = char(lireUintTailleVariable(fichier));

	shared_ptr<Acteur> acteurExistant = listeFilms.trouverActeur(acteur.nom);
	if (acteurExistant != nullptr)
		return acteurExistant;
	else {
		cout << "Création Acteur " << acteur.nom << endl;
		return make_shared<Acteur>(acteur);
	} //TODO: Retourner un pointeur soit vers un acteur existant ou un nouvel acteur ayant les bonnes informations, selon si l'acteur existait déjà.  Pour fins de débogage, affichez les noms des acteurs crées; vous ne devriez pas voir le même nom d'acteur affiché deux fois pour la création.
}

Film* lireFilm(istream& fichier, ListeFilms& listeFilms)
{
	Film* film = new Film;
	film->titre = lireString(fichier);
	film->realisateur = lireString(fichier);
	film->anneeSortie = int(lireUintTailleVariable(fichier));
	film->recette = int(lireUintTailleVariable(fichier));
	int nElements = int(lireUintTailleVariable(fichier)); //sinon on lit trop de fois le fichier
	film->acteurs.changerCapacite(nElements);
	film->acteurs.changerNElements(nElements);  //NOTE: Vous avez le droit d'allouer d'un coup le tableau pour les acteurs, sans faire de réallocation comme pour ListeFilms.  Vous pouvez aussi copier-coller les fonctions d'allocation de ListeFilms ci-dessus dans des nouvelles fonctions et faire un remplacement de Film par Acteur, pour réutiliser cette réallocation.
	 //NOTE: On aurait normalement fait le "new" au début de la fonction pour directement mettre les informations au bon endroit; on le fait ici pour que le code ci-dessus puisse être directement donné aux étudiants sans qu'ils aient le "new" déjà écrit.
	cout << "Création Film " << film->titre << endl;
	/*
	for (int i = 0; i < film.acteurs.nElements; i++) {
	*/
	film->acteurs.changerElements(make_unique<shared_ptr<Acteur>[]>(nElements));
	for (shared_ptr<Acteur>& acteur : film->acteurs.obtenirElements()) {
		acteur = move(lireActeur(fichier, listeFilms)); //TODO: Placer l'acteur au bon endroit dans les acteurs du film.
	}

	return film;

	 //TODO: Retourner le pointeur vers le nouveau film.
}

ListeFilms::ListeFilms(const string& nomFichier) : possedeLesFilms_(true)
{
	ifstream fichier(nomFichier, ios::binary);
	fichier.exceptions(ios::failbit);

	int nElements = int(lireUintTailleVariable(fichier));

	//TODO: Créer une liste de films vide.
	
	/*
	
	for (int i = 0; i < nElements; i++) {
		
	*/
	for ([[maybe_unused]] int i : range(nElements)) { //NOTE: On ne peut pas faire un span simple avec spanListeFilms car la liste est vide et on ajoute des éléments à mesure.
		ajouterFilm(lireFilm(fichier, *this)); //TODO: Ajouter le film à la liste.
	}

}

//TODO: Une fonction pour détruire un film (relâcher toute la mémoire associée à ce film, et les acteurs qui ne jouent plus dans aucun films de la collection).  Noter qu'il faut enleve le film détruit des films dans lesquels jouent les acteurs.  Pour fins de débogage, affichez les noms des acteurs lors de leur destruction.


void detruireFilm(Film* film)
{
	cout << "Destruction Film " << film->titre << endl;
	delete film;
}


//TODO: Une fonction pour détruire une ListeFilms et tous les films qu'elle contient.

//NOTE: Attention que c'est difficile que ça fonctionne correctement avec le destructeur qui détruit la liste.  Mon ancienne implémentation utilisait une méthode au lieu d'un destructeur.  Le problème est que la matière pour le constructeur de copie/move n'est pas dans le TD2 mais le TD3, donc si on copie une liste (par exemple si on la retourne de la fonction creerListe) elle sera incorrectement copiée/détruite.  Ici, creerListe a été converti en constructeur, qui évite ce problème.
ListeFilms::~ListeFilms()
{
	if (possedeLesFilms_)
		for (Film* film : enSpan())
			detruireFilm(film);
	delete[] elements_;
}



//TODO: Une fonction pour afficher un film avec tous ces acteurs (en utilisant la fonction afficherActeur ci-dessus).



void afficherListeFilms(const ListeFilms& listeFilms)
{
	//TODO: Utiliser des caractères Unicode pour définir la ligne de séparation (différente des autres lignes de séparations dans ce progamme).
	static const string ligneDeSeparation = "\033[32m────────────────────────────────────────\033[0m\n";
	cout << ligneDeSeparation;
	//TODO: Changer le for pour utiliser un span.
	/*
	for (int i = 0; i < listeFilms.nElements; i++) {
	*/
	for (const Film* film : listeFilms.enSpan()) {
		//TODO: Afficher le film.
		cout << (*film);
		cout << ligneDeSeparation;
	}
}

ostream& operator<<(ostream& stream, const Acteur& acteur) {
	return stream << "  " << acteur.nom << ", " << acteur.anneeNaissance << " " << acteur.sexe << endl;
}

ostream& operator<<(ostream& stream, const Film& film) {
	stream << "Titre: " << film.titre << endl;
	stream << "  Réalisateur: " << film.realisateur << "  Année :" << film.anneeSortie << endl;
	stream << "  Recette: " << film.recette << "M$" << endl;
	stream << "Acteurs:" << endl;

	for (const shared_ptr<Acteur>& acteur : film.acteurs.obtenirElements())
		stream << *acteur;
	return stream;
}
void lireLivres(const string& nomFichier, vector<variant<Film*, Livre*>>& vecteur) {
	ifstream fichier(nomFichier);
	string ligne;
	while (getline(fichier, ligne)) {
		string titre;
		istringstream stream(ligne);
		stream >> quoted(titre);
		cout << ligne << endl;
	}
}


int main()
{
	bibliotheque_cours::activerCouleursAnsi();  // Permet sous Windows les "ANSI escape code" pour changer de couleurs https://en.wikipedia.org/wiki/ANSI_escape_code ; les consoles Linux/Mac les supportent normalement par défaut.

	//int* fuite = new int; //TODO: Enlever cette ligne après avoir vérifié qu'il y a bien un "Fuite detectee" de "4 octets" affiché à la fin de l'exécution, qui réfère à cette ligne du programme.

	static const string ligneDeSeparation = "\n\033[35m════════════════════════════════════════\033[0m\n";

	//TODO: Chaque TODO dans cette fonction devrait se faire en 1 ou 2 lignes, en appelant les fonctions écrites.

	//TODO: La ligne suivante devrait lire le fichier binaire en allouant la mémoire nécessaire.  Devrait afficher les noms de 20 acteurs sans doublons (par l'affichage pour fins de débogage dans votre fonction lireActeur).
	ListeFilms listeFilms("films.bin");
	vector<variant<Film*, Livre*>> bibliotheque; //plus securitaire que union
	//cout << ligneDeSeparation << "Le premier film de la liste est:" << endl;
	//TODO: Afficher le premier film de la liste.  Devrait être Alien.
	for (Film*& film : listeFilms.enSpan())
		bibliotheque.push_back(film);


	for (variant<Film*, Livre*> item : bibliotheque)
		cout << get<Film*>(item)->titre << endl;
	lireLivres("livres.txt", bibliotheque);
	cout << ligneDeSeparation;
	//test ch 7
	
	//assert(filmString == testCh7);

	

	cout << ligneDeSeparation << "Les films sont:" << endl;
	//TODO: Afficher la liste des films.  Il devrait y en avoir 7.
	
	
	

	//TODO: Modifier l'année de naissance de Benedict Cumberbatch pour être 1976 (elle était 0 dans les données lues du fichier).  Vous ne pouvez pas supposer l'ordre des films et des acteurs dans les listes, il faut y aller par son nom.
	
	
	
	
	//TODO: Afficher la liste des films où Benedict Cumberbatch joue.  Il devrait y avoir Le Hobbit et Le jeu de l'imitation.
	
	// tests ch 7-8
	
	cout << ligneDeSeparation;
	//TODO: Détruire et enlever le premier film de la liste (Alien).  Ceci devrait "automatiquement" (par ce que font vos fonctions) détruire les acteurs Tom Skerritt et John Hurt, mais pas Sigourney Weaver puisqu'elle joue aussi dans Avatar.
	
	//tests ch 10
	
	//tests ch 9
	

	


	
	

	cout << ligneDeSeparation << "Les films sont maintenant:" << endl;
	//TODO: Afficher la liste des films.
	
	
	

	//TODO: Faire les appels qui manquent pour avoir 0% de lignes non exécutées dans le programme (aucune ligne rouge dans la couverture de code; c'est normal que les lignes de "new" et "delete" soient jaunes).  Vous avez aussi le droit d'effacer les lignes du programmes qui ne sont pas exécutée, si finalement vous pensez qu'elle ne sont pas utiles.
	
	// Les lignes à mettre ici dépendent de comment ils ont fait leurs fonctions.  Dans mon cas:
	

	//TODO: Détruire tout avant de terminer le programme.  L'objet verifierFuitesAllocations devrait afficher "Aucune fuite detectee." a la sortie du programme; il affichera "Fuite detectee:" avec la liste des blocs, s'il manque des delete.
	
}
