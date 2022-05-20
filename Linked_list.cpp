#include<iostream>


struct node
{
    int data{0};
    node* prev{nullptr};
    node* next{nullptr};
    node() = default;
    node(int val) : data(val), prev(nullptr), next(nullptr) {}
    node(int val, node* _prev, node* _next) : data(val), prev(_prev), next(_next) {}
};


struct linkedlist
{   
    private:
        int counter{0};
        node* first{nullptr};
        node* last{nullptr};
    public:
        linkedlist() = default;
    
    
    int size() const noexcept
    {
        if(is_empty())
        {
            return 0;
        }
        return counter;
        
    }
    
    
    bool is_empty() const noexcept
    {
        return first == nullptr;
    }
    
    
    void push_back(int const &val) noexcept
    {
        counter++;
        node* p = new node(val);
        if(is_empty())
        {
            first = p;
            last = p;
            return;
        }
        if(first->next == nullptr)
        {
            first->next = p;
            last = p;
            last->prev = first;
            return;
        }
        last->next = p;
        last->next->prev = last;
        last = p;
    }
    
    
    void print() const noexcept
    {
        if(is_empty()) return;
        node* p = first;
        while(p)
        {
            std::cout<<p->data<<' ';
            p = p->next;
        }
        std::cout<<'\n';
    }
    
  node* find_node(int const &val) const noexcept
    {
        if(is_empty()) return nullptr;
        node* p = first;
        while(p)
        {
            if(p->data == val)
            {
                return p;
            }
            p = p->next;
        }
        return nullptr;
    }
    
    
    node* operator[](const int index)
    {
        if (is_empty()) return nullptr;
        node* p = first;
        for (int i = 0; i < index; i++) 
        {
            p = p->next;
            if (!p) 
            { 
                //std::cout<<"Out of list(nullptr)"<<'\n';
                return nullptr;
            }
        }
        return p;
    }
    
    
    void remove_first()
    {
        if (is_empty()) return;
        node* p = first;
        first = p->next;
        delete p;
    }
    
    void remove_last()
    {
        if (is_empty()) return;
        node* p = first;
        while (p->next != last) p = p->next;
        p->next = nullptr;
        delete last;
        last = p;
    }
};
