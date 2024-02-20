#pragma once
// Structures mémoires pour une collection de films.

#include <string>
#include <cassert>
#include <memory>
#include "gsl/span"
using namespace std;
using gsl::span;

struct Film; struct Acteur; // Permet d'utiliser les types alors qu'ils seront définis après.

class ListeFilms {
public:
	ListeFilms() = default;
	ListeFilms(const string& nomFichier);
	ListeFilms(const ListeFilms& l) { assert(l.elements == nullptr); } // Pas demandé dans l'énoncé, mais on veut s'assurer qu'on ne fait jamais de copie de liste, car la copie par défaut ne fait pas ce qu'on veut.  Donc on ne permet pas de copier une liste non vide (la copie de liste vide est utilisée dans la création d'un acteur).
	~ListeFilms();
	void ajouterFilm(Film* film);
	void enleverFilm(const Film* film);
	Acteur* trouverActeur(const string& nomActeur) const;
	span<Film*> enSpan() const;
	int size() const { return nElements; }
	//Film getParCritere(const function<bool(int)>& critere);

private:
	void changeDimension(int nouvelleCapacite);

	int capacite = 0, nElements = 0;
	Film** elements = nullptr; // Pointeur vers un tableau de Film*, chaque Film* pointant vers un Film.
	bool possedeLesFilms_ = false; // Les films seront détruits avec la liste si elle les possède.
};

template<typename T>
class Liste {
public:
	Liste() = default;
	Liste(int capacite)  {
		capacite_ = 1;
		nElements_ = 0;
		elements_ = make_unique<T*[]>(capacite_);
	}
	Liste(const Liste<T>& l) {
		capacite_ = l.capacite_;
		nElements_ = l.nElements_;
		elements_ = make_unique<T* []>(l.capacite_);
	}
	~Liste(){}
	unique_ptr<T* []> getElements() {
		return elements_;
	}
	int getNElements() {
		return nElements_;
	}
	int getCapacite() {
		return capacite_;
	}
	void setElements(unique_ptr<T* []> elements) {
		elements_ = move(elements);
	}
	void setNElements(int valeur) {
		nElements_ = valeur;
	}
	void setCapacite(int valeur) {
		capacite_ = valeur;
	}
private:
	int capacite_, nElements_;
	unique_ptr<T* []> elements_;
};
using ListeActeurs = Liste<Acteur>;

struct Film
{
	string titre, realisateur; // Titre et nom du réalisateur (on suppose qu'il n'y a qu'un réalisateur).
	int anneeSortie, recette; // Année de sortie et recette globale du film en millions de dollars
	ListeActeurs acteurs;
};

struct Acteur
{
	string nom; int anneeNaissance; char sexe;
	ListeFilms joueDans;
};
