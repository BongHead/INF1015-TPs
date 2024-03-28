/**
* En-tête du programme tp4.cpp.
* \file   structures.hpp
* \author Song Ning Lan et Sheng He Ge
* \date   17 mars 2024
* Créé le 29 janvier 2024
*/

#pragma once
// Structures mémoires pour une collection de films.

#include "cppitertools/range.hpp"
#include "gsl/span"
#include <cassert>
#include <functional>
#include <iostream>
#include <memory>
#include <string>

using gsl::span;
using namespace std;
using namespace iter;

class Film; struct Acteur;
class ListeFilms {
public:
	ListeFilms() = default;
	ListeFilms(const string& nomFichier);
	ListeFilms(const ListeFilms& l) { assert(l.elements_ == nullptr); }
	void ajouterFilm(Film* film);
	~ListeFilms();
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
	Film** elements_ = nullptr;
	bool possedeLesFilms_ = false;
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

	class Iterator {
	public:
		Iterator(int position, Liste<T>& liste) : position_(position), liste_(liste) {}

		shared_ptr<T>& operator*() {
			return liste_.elements_[position_];
		}
		Iterator& operator++() {
			position_++;
			return *this;
		}
		bool operator==(const Iterator& autre) const {
			return position_ == autre.position_;
		}
	private:
		int position_;
		Liste<T>& liste_;
	};

	Iterator begin() {
		return Iterator(0, *this);
	}
	Iterator end() {
		return Iterator(nElements_, *this);
	}
	
private:
	int capacite_ = 0, nElements_ = 0;
	unique_ptr<shared_ptr<T>[]> elements_;
};
using ListeActeurs = Liste<Acteur>;



class Affichable {
public:
	virtual ~Affichable(){}
	virtual void afficher(bool avecItem) const = 0;
};

class Item: virtual public Affichable{
public:
	friend shared_ptr<Acteur> ListeFilms::trouverActeur(const string& nomActeur) const;
	friend Film* lireFilm(istream& fichier, ListeFilms& listeFilms);
	friend void detruireFilm(Film* film);
	Item() = default;
	Item(const string& titre, int annee): titre(titre), anneeSortie(annee){}
	virtual ~Item(){}

	virtual void afficher(bool avecItem) const override;
	
private:
	string titre = "";
	int anneeSortie = 0;
};

class Film : virtual public Item
{
public:
	Film() = default;
	friend shared_ptr<Acteur> ListeFilms::trouverActeur(const string& nomActeur) const;
	friend Film* lireFilm(istream& fichier, ListeFilms& listeFilms);
	friend void detruireFilm(Film* film);

	ListeActeurs obtenirActeurs() {
		return acteurs;
	}
	virtual ~Film(){}

	virtual void afficher(bool avecItem) const override;
	

private:
	string realisateur = "";
	int recette = 0;
	ListeActeurs acteurs;

	
};

struct Acteur
{
	string nom = ""; int anneeNaissance = 0; char sexe = ' ';
	friend ostream& operator<<(ostream& stream, const Acteur& acteur);
};


class Livre : virtual public Item {
public:
	Livre(const string& titre, int annee, const string& auteur, int nCopies, int nPages)
		:Item(titre, annee), auteur(auteur), copiesVendues(nCopies), nPages(nPages) {}

	virtual void afficher(bool avecItem) const override;
	
	virtual ~Livre(){}
	
private:
	string auteur = "";
	int copiesVendues = 0;
	int nPages = 0;
};

class FilmLivre : public Film, public Livre {
public:
	FilmLivre(const Film& film, const Livre& livre):
		Item(film), Film(film), Livre(livre){}
	~FilmLivre(){}

	void afficher(bool avecItem) const override;
};
