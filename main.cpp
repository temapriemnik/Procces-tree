#include <iostream>
#include <vector>
#include <string>
#include <exception>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

using namespace std;

class my_exp : public exception {
private:
    string message;

public:
    my_exp(const string& msg) : message(msg) {}

    virtual const char* what() const noexcept override {
        return message.c_str();
    }
};

class node {
private:
    int id;
    pid_t pid;
    node* parent;
    vector<node*> children;

public:
    node() : id(0), parent(nullptr), pid(0) {}

    node(int id) : id(id), parent(nullptr), pid(0) {}

    node(node* parent, int id, pid_t pid) : parent(parent), id(id), pid(pid) {}

    void add_child(node* child) {
        this->children.push_back(child);
    }

    void set_pid(pid_t pid) {
        this->pid = pid;
    }

    pid_t get_pid() {
        return this->pid;
    }

    int get_id() {
        return this->id;
    }

    const vector<node*>& get_children() const {
        return children;
    }
};

class tree {
private:
    node* head;
    int max_id;
    node* current;

public:
    tree() {
        max_id = 0;
        head = new node(0);
        head->set_pid(getpid());
        current = head;
    }

    node* get_head() {
        return this->head;
    }

    node* find_by_id(int id, node* current) {
        if (current->get_id() == id) {
            return current;
        }
        for (auto i : current->get_children()) {
            node* found = find_by_id(id, i);
            if (found) return found;
        }
        return nullptr;
    }

    void add_child(node* parent) {
        pid_t pid = fork();
        max_id++;
        node* child = new node(parent, max_id, pid);
        parent->add_child(child);
    }

    void Ps(int target_id) {
        node* n = find_by_id(target_id, head);
        if (!n) {
            cerr << "Node not found" << endl;
            return;
        }
        pid_t target_pid = n->get_pid();

        pid_t pid = fork();

        if (pid == 0) {
            execlp("ps", "ps", "-p", to_string(target_pid).c_str(), nullptr);
            cerr << "ERROR with exec" << endl;
            exit(1);
        } else if (pid > 0) {
            wait(nullptr);
        } else {
            cerr << "ERROR with making new process" << endl;
        }
    }
};

int main() {
    tree test;

    test.add_child(test.get_head());
    test.add_child(test.get_head());
    test.add_child(test.get_head());
    test.add_child(test.get_head());
    test.add_child(test.get_head());
    test.add_child(test.get_head());
    test.add_child(test.get_head());

    test.Ps(7);

    return 0;
}
