
#include <iostream>
#include <string>
#include <windows.h>
using namespace std;

class Employee {
protected:
    string name;
    int baseSalary;

public:
    Employee(const string& n, int s) : name(n), baseSalary(s) {}

    string getName() const { return name; }
    int getSalary() const { return baseSalary; }

    void print() const {
        cout << name << " (ставка: " << baseSalary << " грн)";
    }
};

class Developer : virtual public Employee {
private:
    string language;
    int tasksDone;

public:
    Developer(const string& n, int s, const string& l)
        : Employee(n, s), language(l), tasksDone(0) {
    }

    void completeTask() { tasksDone++; }
    int getTasksDone() const { return tasksDone; }

    int getSalary() const {
        return baseSalary + tasksDone * 1000;
    }

    void print() const {
        Employee::print();
        cout << ", " << language << ", задач: " << tasksDone
            << ", зарплата: " << getSalary() << " грн\n";
    }
};

class Manager : virtual public Employee {
private:
    Developer** team;
    int teamSize;

public:
    Manager(const string& n, int s)
        : Employee(n, s), team(nullptr), teamSize(0) {
    }

    Manager(const Manager&) = delete;
    Manager& operator=(const Manager&) = delete;

    ~Manager() { delete[] team; }

    bool addToTeam(Developer* dev) {
        if (!dev) return false;

        Developer** temp = new Developer * [teamSize + 1];

        for (int i = 0; i < teamSize; i++)
            temp[i] = team[i];

        temp[teamSize++] = dev;
        delete[] team;
        team = temp;
        return true;
    }

    int getTeamTasks() const {
        int sum = 0;
        for (int i = 0; i < teamSize; i++)
            sum += team[i]->getTasksDone();
        return sum;
    }

    int getSalary() const {
        return baseSalary + teamSize * 2000;
    }

    void showTeam() const {
        for (int i = 0; i < teamSize; i++)
            team[i]->print();

        cout << "Всего задач команды: " << getTeamTasks() << "\n";
    }

    void print() const {
        Employee::print();
        cout << ", зарплата: " << getSalary() << " грн\n";
        showTeam();
    }
};

class TeamLead : public Developer, public Manager {
public:
    TeamLead(const string& n, int s, const string& l)
        : Employee(n, s), Developer(n, s, l), Manager(n, s) {
    }

    int getSalary() const {
        return Developer::getSalary()
            + Manager::getSalary() - baseSalary;
    }

    void print() const {
        Employee::print();
        cout << ", своих задач: " << getTasksDone()
            << ", зарплата: " << getSalary() << " грн\n";
        showTeam();
    }
};

int main() {
    SetConsoleOutputCP(65001);
    SetConsoleCP(65001);

    Developer d1("Елена", 30000, "C++");
    Developer d2("Иван", 28000, "Python");

    for (int i = 0; i < 3; i++) d1.completeTask();
    d2.completeTask();

    d1.print();
    d2.print();
    cout << "Имя: " << d1.getName() << "\n\n";

    Developer d3("Анна", 32000, "Java");
    Manager manager("Олег", 40000);

    manager.addToTeam(&d1);
    manager.addToTeam(&d2);
    manager.addToTeam(&d3);

    d3.completeTask();
    d3.completeTask();

    manager.print();
    cout << endl;

    TeamLead lead("Максим", 50000, "C++");

    lead.completeTask();
    lead.completeTask();

    lead.addToTeam(&d1);
    lead.addToTeam(&d2);

    lead.print();
    cout << "Имя тимлида: " << lead.getName() << endl;

    return 0;
}

/*

1. protected позволяет наследникам обращаться к полям
   напрямую. При private пришлось бы использовать
   методы доступа Employee.

2. Ошибка компиляции: у Employee нет конструктора
   без параметров, а базовый класс нужно создать.

3. Сначала Employee, затем Developer.
   Деструкторы вызываются в обратном порядке.

4. Вызовется Developer::getSalary().
   Для базовой версии: Employee::getSalary().

5. Manager является Employee — наследование.
   Manager содержит указатели на Developer — агрегация.
   delete[] освобождает массив, но не разработчиков,
   потому что они принадлежат main.

6. Без virtual было бы два поля name:
   одно через Developer, другое через Manager.
   getName() был бы неоднозначным.

7. TeamLead сам создаёт общий виртуальный Employee.
   Вызовы Employee(...) внутри Developer и Manager
   при создании TeamLead игнорируются.
*/
