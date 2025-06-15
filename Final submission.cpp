#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <algorithm>
#include <limits>
#include <random>

using namespace std;

class FlashCard {
public:
    string question;
    string answer;
    int difficulty;

    FlashCard() {
        difficulty = 1;
    }

    FlashCard(string q, string a) {
        question = q;
        answer = a;
        difficulty = 1;
    }

    void markDifficulty(bool correct) {
        if (correct)
            difficulty = 0;
        else
            difficulty = 2;
    }

    void saveToFile(ofstream &out) {
        size_t qSize = question.size();
        size_t aSize = answer.size();
        out.write((char*)&qSize, sizeof(qSize));
        out.write(question.c_str(), qSize);
        out.write((char*)&aSize, sizeof(aSize));
        out.write(answer.c_str(), aSize);
        out.write((char*)&difficulty, sizeof(difficulty));
    }

    void loadFromFile(ifstream &in) {
        size_t qSize, aSize;
        in.read((char*)&qSize, sizeof(qSize));
        question.resize(qSize);
        in.read(&question[0], qSize);
        in.read((char*)&aSize, sizeof(aSize));
        answer.resize(aSize);
        in.read(&answer[0], aSize);
        in.read((char*)&difficulty, sizeof(difficulty));
    }
};

class Deck {
public:
    vector<FlashCard> cards;

    void addCard(string q, string a) {
        FlashCard card(q, a);
        cards.push_back(card);
    }

    void save(string filename) {
        ofstream out(filename, ios::binary);
        size_t size = cards.size();
        out.write((char*)&size, sizeof(size));
        for (int i = 0; i < size; i++) {
            cards[i].saveToFile(out);
        }
        out.close();
    }

    void load(string filename) {
        ifstream in(filename, ios::binary);
        if (!in) {
            cout << "File not found.\n";
            return;
        }

        cards.clear();
        size_t size;
        in.read((char*)&size, sizeof(size));
        for (size_t i = 0; i < size; i++) {
            FlashCard card;
            card.loadFromFile(in);
            cards.push_back(card);
        }
        in.close();
    }
};

class UserProgress {
public:
    int totalReviewed;
    int correctAnswers;

    UserProgress() {
        totalReviewed = 0;
        correctAnswers = 0;
    }

    void update(bool correct) {
        totalReviewed++;
        if (correct) {
            correctAnswers++;
        }
    }

    void show() {
        cout << "\n--- Progress ---\n";
        cout << "Total reviewed: " << totalReviewed << endl;
        cout << "Correct: " << correctAnswers << endl;
        if (totalReviewed > 0) {
            double percent = (double)correctAnswers / totalReviewed * 100;
            cout << "Accuracy: " << percent << "%" << endl;
        } else {
            cout << "No reviews yet.\n";
        }
    }
};

class ReviewHelper {
public:
    vector<FlashCard> prepareSet(vector<FlashCard> &cards) {
        vector<FlashCard> review;
        for (int i = 0; i < cards.size(); i++) {
            int times = 1;
            if (cards[i].difficulty == 2) {
                times = 3;
            } else if (cards[i].difficulty == 1) {
                times = 2;
            }
            for (int j = 0; j < times; j++) {
                review.push_back(cards[i]);
            }
        }

        random_device rd;
        default_random_engine eng(rd());
        shuffle(review.begin(), review.end(), eng);

        return review;
    }
};

class FlashCardApp {
private:
    Deck deck;
    UserProgress progress;
    ReviewHelper helper;

public:
    void run() {
        deck.load("cards.dat");
        int choice;

        while (true) {
            cout << "\nDigital Flashcard\n";
            cout << "1) Create new flashcard\n";
            cout << "2) Review flashcards\n";
            cout << "3) Show progress\n";
            cout << "4) Exit\n";
            cout << "Choose: ";
            cin >> choice;

            if (cin.fail()) {
                cin.clear(); // clear the error state
                cin.ignore(numeric_limits<streamsize>::max(), '\n'); // discard bad input
                cout << "Invalid input. Please enter a number between 1 and 4.\n";
                continue;
            }

            cin.ignore(numeric_limits<streamsize>::max(), '\n'); // flush input

            if (choice == 1) {
                makeCard();
            } else if (choice == 2) {
                reviewCards();
            } else if (choice == 3) {
                progress.show();
            } else if (choice == 4) {
                deck.save("cards.dat");
                cout << "Goodbye!\n";
                break;
            } else {
                cout << "Invalid input. Please choose 1-4.\n";
            }
        }
    }

    void makeCard() {
        string q, a;
        cout << "Enter question: ";
        getline(cin, q);
        cout << "Enter answer: ";
        getline(cin, a);
        deck.addCard(q, a);
        cout << "Card created.\n";
    }

    void reviewCards() {
        if (deck.cards.empty()) {
            cout << "No flashcards available.\n";
            return;
        }

        vector<FlashCard> review = helper.prepareSet(deck.cards);
        for (int i = 0; i < review.size(); i++) {
            cout << "\nQ: " << review[i].question << endl;
            cout << "See answer? (y/n): ";
            char ch;
            cin >> ch;
            cin.ignore();
            if (ch == 'y' || ch == 'Y') {
                cout << "A: " << review[i].answer << endl;
            }

            cout << "Did you know the answer? (y/n): ";
            char res;
            cin >> res;
            cin.ignore();
            bool knewIt = (res == 'y' || res == 'Y');
            review[i].markDifficulty(knewIt);
            progress.update(knewIt);

            cout << "1) Next  2) Main menu\nChoose: ";
            int next;
            cin >> next;
            cin.ignore();
            if (next != 1) {
                break;
            }
        }
    }
};

int main() {
    FlashCardApp app;
    app.run();
    return 0;
}
