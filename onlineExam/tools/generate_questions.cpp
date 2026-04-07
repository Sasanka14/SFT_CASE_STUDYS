#include <iostream>
#include <fstream>
#include <vector>
#include <string>

struct Q {
    std::string q,a,b,c,d;
    int correct;
    std::string diff,cat;
};

int main() {
    std::ofstream out("../data/questions.txt");

    std::vector<Q> base = {
        {"What is the capital of France?","Berlin","Madrid","Paris","Rome",2,"easy","geography"},
        {"Which language is used for C++?","Python","C++","Java","Go",1,"easy","programming"},
        {"2 + 2 equals?","3","4","5","6",1,"easy","math"},
        {"Which planet is known as the Red Planet?","Earth","Mars","Venus","Jupiter",1,"easy","science"},
        {"HTML stands for?","Hyper Trainer Marking Language","Hyper Text Markup Language","High Text Machine Language","None",1,"easy","web"}
    };

    int target = 500;
    int id = 0;

    while (id < target) {
        for (auto &q : base) {
            if (id >= target) break;

            out << q.q << "|"
                << q.a << "|"
                << q.b << "|"
                << q.c << "|"
                << q.d << "|"
                << q.correct << "|"
                << q.diff << "|"
                << q.cat << "\n";

            id++;
        }
    }

    std::cout << "✅ Generated " << id << " questions\n";
}