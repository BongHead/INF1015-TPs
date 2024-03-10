/**
* En-tête du programme tp3.cpp.
* \file   structures.hpp
* \author Song Ning Lan et Sheng He Ge
* \date   25 février 2024
* Créé le 29 janvier 2024
*/

#pragma once
// Structures mémoires pour une collection de films.

#include <string>
#include <cassert>
#include <memory>
#include <functional>
#include <iostream>
#include "cppitertools/range.hpp"
#include "gsl/span"

using gsl::span;
using namespace std;
using namespace iter;

class Film; struct Acteur; // Permet d'utiliser les types alors qu'ils seront définis après.

class ListeFilms {
public:
	ListeFilms() = default;
	ListeFilms(const string& nomFichier);
	ListeFilms(const ListeFilms& l) { assert(l.elements_ == nullptr); } // Pas demandé dans l'énoncé, mais on veut s'assurer qu'on ne fait jamais de copie de liste, car la copie par défaut ne fait pas ce qu'on veut.  Donc on ne permet pas de copier une liste non vide (la copie de liste vide est utilisée dans la création d'un acteur).
	~ListeFilms();
	void ajouterFilm(Film* film);
	void enleverFilm(const Film* film);
	shared_ptr<Acteur> trouverActeur(const string& nomActeur) const;
	span<Film*> enSpan() const;
	int size() const { return nElements_; }
	Film*& operator[](int index) const {
		return elements_[index];
	}
	Film* trouverParCritere(const function<bool(const Film&)>& critere) const{
		for (Film*& film : enSpan()) {
			if (critere(*film))
				return film;
		}
		return nullptr;
	}

private:
	void changeDimension(int nouvelleCapacite);

	int capacite_ = 0, nElements_ = 0;
	Film** elements_ = nullptr; // Pointeur vers un tableau de Film*, chaque Film* pointant vers un Film.
	bool possedeLesFilms_ = false; // Les films seront détruits avec la liste si elle les possède.
};


template<typename T>
class Liste {
public:
	Liste() = default;
	Liste(int capacite):capacite_(capacite), elements_(make_unique<shared_ptr<T>[]>(capacite)) {}
	Liste(const Liste<T>& liste) :capacite_(liste.capacite_), nElements_(liste.nElements_), elements_(make_unique<shared_ptr<T>[]>(capacite_)) {
		for (int i : range(nElements_)) {
			elements_[i] = liste.elements_[i];
		}
	}
	~Liste(){}
	span<shared_ptr<T>> obtenirElements() const {
		return span(elements_.get(), nElements_);
	}
	void changerCapacite(int capacite) {
		capacite_ = capacite;
	}
	void changerNElements(int nElements) {
		nElements_ = nElements;
	}
	void changerElements(unique_ptr<shared_ptr<T>[]> liste) {
		elements_ = move(liste);
	}

	int obtenirNElements() {
		return nElements_;
	}
	shared_ptr<T>& operator[](int index) const {
		return elements_[index];
	}
	void ajouterElement(shared_ptr<T> element) {
		assert(capacite_ > nElements_);
		elements_[nElements_] = move(element);
		nElements_++;
	}
private:
	int capacite_ = 0, nElements_ = 0;
	unique_ptr<shared_ptr<T>[]> elements_; // Pointeur vers un tableau de Acteur*, chaque Acteur* pointant vers un Acteur.
};
using ListeActeurs = Liste<Acteur>;


class Item {
public:
	string titre = "";
	int anneeSortie = 0;
private:
};

class Film : virtual public Item
{
public:
	string realisateur = ""; // Titre et nom du réalisateur (on suppose qu'il n'y a qu'un réalisateur).
	int recette = 0; // Année de sortie et recette globale du film en millions de dollars
	ListeActeurs acteurs;

	friend shared_ptr<Acteur> trouverActeur(const string& nomActeur);
	friend ostream& operator<<(ostream& stream, const Film& film);
	ListeActeurs& obtenirActeurs() {
		return acteurs;
	}
private:
	

	
};

struct Acteur
{
	string nom = ""; int anneeNaissance = 0; char sexe = ' ';
	friend ostream& operator<<(ostream& stream, const Acteur& acteur);
};


class Livre : virtual public Item {
public:
	string auteur = "";
	int copiesVendues = 0;
	int nPages = 0;
private:
	
};

class FilmLivre : public Film, public Livre {
	
};
