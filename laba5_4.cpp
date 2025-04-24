#include <iostream>
#include <map>
#include <set>
#include <string>

using namespace std;

int main() {
    int N;
    cin >> N;

    // Храним друзей для каждого человека
    map<string, set<string>> friends;

    for (int i = 0; i < N; ++i) {
        string cmd;
        cin >> cmd;

        if (cmd == "FRIENDS") {
            string a, b;
            cin >> a >> b;
            friends[a].insert(b);
            friends[b].insert(a);
        }
        else if (cmd == "COUNT") {
            string name;
            cin >> name;
            cout << friends[name].size() << endl;
        }
        else if (cmd == "QUESTION") {
            string a, b;
            cin >> a >> b;
            if (friends[a].count(b) > 0) {
                cout << "YES" << endl;
            } else {
                cout << "NO" << endl;
            }
        }
    }

    return 0;
}
