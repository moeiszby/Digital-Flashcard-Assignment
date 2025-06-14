#include <iostream>
#include <vector>
#include <string>
using namespace std;

class FlashCard {
public:
    string question;
    string answer;
};

int main() {
    vector<FlashCard> cards;
    int option;

    while (true) {
        cout << "\n1) Create card\n2) Show cards\n3) Exit\nChoose: ";
        cin >> option;
        cin.ignore();

        if (option == 1) {
            FlashCard card;
            cout << "Enter question: ";
            getline(cin, card.question);
            cout << "Enter answer: ";
            getline(cin, card.answer);
            cards.push_back(card);
        } else if (option == 2) {
            for (int i = 0; i < cards.size(); i++) {
                cout << "\nQ: " << cards[i].question << "\nA: " << cards[i].answer << endl;
            }
        } else {
            break;
        }
    }

    return 0;
}
