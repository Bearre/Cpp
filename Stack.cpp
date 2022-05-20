#include<iostream>


struct node
{
    int data{0};
    node* next{nullptr};
    node() = default;
    node(int val) : data(val), next(nullptr) {}
    node(int val, node* _next) : data(val), next(_next) {}
};


struct stack
{
    node* top{nullptr};
    stack() = default;
    
    
    bool is_empty()
    {
        return top == nullptr;
    }
    
    
    void push_back(int const val)
    {
        if(is_empty())
        {
            top = new node(val);
        }
        node* p = new node(val);
        p->next = top;
        top = p;
    }
    
    
    int size()
    {
        if(is_empty()) return 0;
        node* p = top;
        int counter = 0;
        while(p->next)
        {
            p = p->next;
            counter++;
        }
        return counter;
    }
    
    
    int pop()
    {
        if (is_empty()) return 0;
        node* p = top;
        int data = p->data;
        top = p->next;
        delete p;
        return data;
    }
};

int main()
{
    stack s;
    s.push_back(4);
    s.push_back(2);
    s.push_back(3);
    s.push_back(0);
    cout<<s.size()<<'\n';
    cout<<s.top->data;
    cout<<s.pop();
    cout<<s.top->data;
    cout<<s.top->next->data;
    return 0;
}
