/**
* Programme qui reprend le tp3 et qui apporte de nombreuses modifications à celles-ci, l'héritage virtuel et mutiple.
* \file   tp4.cpp
* \author Song Ning Lan et Sheng He Ge
* \date   17 mars 2024
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



void ListeFilms::changeDimension(int nouvelleCapacite)
{
	Film** nouvelleListe = new Film* [nouvelleCapacite];

	if (elements_ != nullptr) { 
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



span<Film*> ListeFilms::enSpan() const { return span(elements_, nElements_); }


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
	} 
}

Film* lireFilm(istream& fichier, ListeFilms& listeFilms)
{
	Film* film = new Film;
	film->titre = lireString(fichier);
	film->realisateur = lireString(fichier);
	film->anneeSortie = int(lireUintTailleVariable(fichier));
	film->recette = int(lireUintTailleVariable(fichier));
	int nElements = int(lireUintTailleVariable(fichier));
	film->acteurs.changerCapacite(nElements);
	film->acteurs.changerNElements(nElements);
	cout << "Création Film " << film->titre << endl;

	film->acteurs.changerElements(make_unique<shared_ptr<Acteur>[]>(nElements));
	for (shared_ptr<Acteur>& acteur : film->acteurs.obtenirElements()) {
		acteur = move(lireActeur(fichier, listeFilms));
	}

	return film;

}

ListeFilms::ListeFilms(const string& nomFichier) : possedeLesFilms_(true)
{
	ifstream fichier(nomFichier, ios::binary);
	fichier.exceptions(ios::failbit);

	int nElements = int(lireUintTailleVariable(fichier));

	for ([[maybe_unused]] int i : range(nElements)) {
		ajouterFilm(lireFilm(fichier, *this));
	}

}




void detruireFilm(Film* film)
{
	cout << "Destruction Film " << film->titre << endl;
	delete film;
}


ListeFilms::~ListeFilms()
{
	if (possedeLesFilms_)
		for (Film* film : enSpan())
			detruireFilm(film);
	delete[] elements_;
}




ostream& operator<<(ostream& stream, const Acteur& acteur) {
	return stream << "  " << acteur.nom << ", " << acteur.anneeNaissance << " " << acteur.sexe << endl;
}




void Item::afficher(bool avecItem) const {
	if (avecItem) {
		cout << "Titre: " << titre << endl;
		cout << "Annee: " << anneeSortie << endl;
	}
}
ostream& operator<<(ostream& stream, const Item& item) {
	item.afficher(true);
	stream << "\n----------------------------\n";
	return stream;
}

void Film::afficher(bool avecItem) const {
	Item::afficher(avecItem);
	cout << "Realisateur: " << realisateur << endl;
	cout << "Recette: " << recette << "M$" << endl;
	cout << "Acteurs: " << endl;
	for (auto&& acteur: acteurs.obtenirElements()) {
		cout << (*acteur);;
	}
}


void Livre::afficher(bool avecItem) const {
	
	Item::afficher(avecItem);
	cout << "Auteur: " << auteur << endl;
	cout << "Copies vendues: " << copiesVendues << "M" << endl;
	cout << "Nombre de pages: " << nPages << endl;
}



void FilmLivre::afficher(bool avecItem) const {
	cout << "Film:" << endl;
	Film::afficher(true);
	cout << "Livre:" << endl;
	Livre::afficher(false);
}

void afficherListeFilms(const vector<unique_ptr<Item>>& vecteur)
{
	static const string ligneDeSeparation = "\033[32m────────────────────────────────────────\033[0m\n";
	cout << ligneDeSeparation;
	for (auto&& item : vecteur) {
		cout << (*item);
	}
}

void lireLivres(const string& nomFichier, vector<unique_ptr<Item>>& vecteur) {
	ifstream fichier(nomFichier);
	string ligne;
	while (getline(fichier, ligne)) {
		string titre, anneeString, auteur, copiesString, pagesString;
		istringstream iss(ligne);
		iss >> quoted(titre) >> quoted(anneeString) >> quoted(auteur) >> quoted(copiesString) >> quoted(pagesString);
		int annee = stoi(anneeString);
		int copies = stoi(copiesString);
		int pages = stoi(pagesString);
		vecteur.push_back(make_unique<Livre>(titre, annee, auteur, copies, pages));
	}
}


int main()
{
	bibliotheque_cours::activerCouleursAnsi();  // Permet sous Windows les "ANSI escape code" pour changer de couleurs https://en.wikipedia.org/wiki/ANSI_escape_code ; les consoles Linux/Mac les supportent normalement par défaut.

	

	static const string ligneDeSeparation = "\n\033[35m════════════════════════════════════════\033[0m\n";

	
	ListeFilms listeFilms("films.bin");
	vector<unique_ptr<Item>> bibliotheque;
	
	for (Film* film : listeFilms.enSpan())
		bibliotheque.push_back(make_unique<Film>(*film));


	
	lireLivres("livres.txt", bibliotheque);
	
	cout << ligneDeSeparation;
	// test afficher
	afficherListeFilms(bibliotheque);

	// test FilmLivre
	Film hobbitFilm(*dynamic_cast<Film*>(bibliotheque.at(4).get()));
	Livre hobbitLivre(*dynamic_cast<Livre*>(bibliotheque.at(10).get()));
	FilmLivre a(hobbitFilm, hobbitLivre);
	bibliotheque.push_back(make_unique<FilmLivre>(hobbitFilm, hobbitLivre));

	// test affichage FilmLivre
	cout << (*bibliotheque.back()); //dernier element
}
