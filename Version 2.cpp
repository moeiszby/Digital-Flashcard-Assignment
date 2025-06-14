#include <iostream>
#include <vector>
#include <string>
using namespace std;

class FlashCard {
public:
    string question;
    string answer;
    bool known;
};

int main() {
    vector<FlashCard> cards;
    int option;

    while (true) {
        cout << "\n1) Add card\n2) Review cards\n3) Exit\nChoose: ";
        cin >> option;
        cin.ignore();

        if (option == 1) {
            FlashCard c;
            cout << "Question: ";
            getline(cin, c.question);
            cout << "Answer: ";
            getline(cin, c.answer);
            c.known = false;
            cards.push_back(c);
        } else if (option == 2) {
            for (int i = 0; i < cards.size(); i++) {
                cout << "\nQ: " << cards[i].question << "\nShow answer? (y/n): ";
                char ch;
                cin >> ch;
                cin.ignore();
                if (ch == 'y') {
                    cout << "A: " << cards[i].answer << endl;
                }

                cout << "Did you know the answer? (y/n): ";
                cin >> ch;
                cin.ignore();
                cards[i].known = (ch == 'y');
            }
        } else {
            break;
        }
    }

    return 0;
}
