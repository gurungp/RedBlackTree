#include <iostream>

enum class Color{
    Red,
    Black
};

enum class NodeCases{
    LeftLeft,
    RightRight,
    LeftRight,
    RightLeft
};

struct node{
    int val;
    Color color;
    node* parent;
    node* left;
    node* right;
    node(int v):val(v),color(Color::Red),left(nullptr),right(nullptr){}
    char* color_name()
    {
        if (color==Color::Black)
        {
            return "BLACK";
        }else{
            return "RED";
        }
    }

};

class RBTree{
public:
    RBTree(){
        
    }
    RBTree(int v){
        root = new node(v);
        root->parent = nullptr;
        root->color = Color::Black;
        size++;
    }

    ~RBTree(){
        deleteTree(root);
    }

    bool contains(int val)
    {
        node* traverse = root;
        
        while (traverse!=nullptr)
        {
            if (val==traverse->val)
            {
                return true;
            }
            else if (val<traverse->val)
            {
                traverse=traverse->left;
            }
            else if (val>traverse->val)
            {
                traverse=traverse->right;
            }
            
            
        }
        return false;
    }
    
    node* find(int val) // Returns the parent
    {
        node* traverse = root;
        
        while(traverse!=nullptr)
        {   
            if(val==traverse->val)
            { 
                return traverse;
            }
            else if (val<traverse->val)
            {
                traverse=traverse->left;
            }
            else
            {
                traverse = traverse->right;
            }
        }
        return nullptr;
    }
    
    void insertNode(int v)
    {
        if (root==nullptr) // Case 1
        {
            root = new node(v);
            root->parent = nullptr;
            root->color = Color::Black;
        }
        else{
            root = insertNode(root,v);
        }
        size++;
    }

    node* insertNode(node *n,int v)
    {
        if(n==nullptr)
        {
            return new node(v);
        }
        else{
            if (n->val<v)
            {
                node* temp = insertNode(n->right,v);
                node* uncle = nullptr;
                if(temp!=nullptr and n!=temp){                    
                    temp->parent = n;
                    n->right = temp;
                    if(n!=root or n->parent!=nullptr)
                        uncle = (n->parent->val < n->val) ? n->parent->left : n->parent->right;
                    
                    return update2(n,n->right,uncle);
                }
                
                return n->parent;
            }
            else
            {   
                node* temp = insertNode(n->left,v);
                node* uncle = nullptr;
                
                if(temp!=nullptr and n!=temp){                    
                    temp->parent = n;
                    n->left = temp;
                    
                    if(n!=root or n->parent!=nullptr)
                        uncle = (n->parent->val < n->val) ? n->parent->left : n->parent->right;
                    
                    return update2(n,n->left,uncle);
                }

                return n->parent;

            }
        }

    }

    node* update2(node* n,node* c,node* uncle)
    {
        if(n==nullptr or c==nullptr)
            return nullptr;
        
        if (n->color == Color::Red && c->color==Color::Red) //uncle == nullptr is checked first,
                                                            //preventing a possible segmentation fault
                                                            //from dereferencing a nullptr when checking the 2nd condition
        {
            // Rotate and recolor
            if (uncle==nullptr or uncle->color==Color::Black)
            {
                // Case 3
                NodeCases nc = findCase(n,c);
                if(nc==NodeCases::RightLeft)
                {
                    
                    node* res1 = rightleftRotation(n,c);
                    // node* res2 = rightrightRotation(n,c); // note n and c are previous ones before rotation
                    node* res2 = rightrightCase(c,n); // note n and c are previous ones before rotation

                    // res1->color = Color::Black;
                    // res2->color = Color::Red;
                    return nullptr;
    
                }
                if(nc==NodeCases::RightRight)
                {
                    node* res1 = rightrightCase(n,c);
                    return nullptr;
                }
                if(nc==NodeCases::LeftRight)
                {
                    node* res1 = leftrightRotation(n,c);

                    node* res2 = leftleftCase(c,n); // note n and c are previous ones before rotation

                    // res1->color = Color::Black;
                    // res2->color = Color::Red;
                    return nullptr;
                }
                if(nc==NodeCases::LeftLeft)
                {
                    node* res1 = leftleftCase(n,c);
                    return nullptr;
                }
               
            }
            else // if uncle's color is Red
            { 
                // CASE 2
                if(uncle!=nullptr)
                {
                    uncle->color = Color::Black;
                    n->color=Color::Black;
                }
                else{
                    n->color=Color::Black;
                }
                if(n->parent!=root)
                    n->parent->color = Color::Red;

                return n;
            }
            
            
        }
        return n;
    }

    NodeCases findCase(node* n,node* c)
    {
        if (n->val < n->parent->val) // Left to its parent
        {
            if (n->val > c->val) // Child is left to the node
            {
                return NodeCases::LeftLeft;
            }
            else // Child is right to the node
            {
                return NodeCases::LeftRight;
            }
        }
        else // Right to its parent 
        {
            if (n->val < c->val) // Child is right to the node
            {
                return NodeCases::RightRight;
            }
            else // Child is left to the node
            {
                return NodeCases::RightLeft;
            }
        }
    }
    
    
    node* rightrightCase(node* n,node* c)
    {
        node* p = n->parent;
        if(p!=root)
        {
            node* g = p->parent;
            if(g->val < p->val)
                g->right = n;
            else
                g->left = n;

            n->parent = g;
            //return p
        }
        p->parent = n;
        p->right =n->left;
        if(n->left!=nullptr)
            n->left->parent = p;
        n->left = p;

        p->color = Color::Red;
        n->color = Color::Black;

        return p;
    }
    node* leftleftCase(node* n,node* c)
    {
        node* p = n->parent;
        if(p!=root)
        {
            node* g = p->parent;
            if(g->val < p->val)
                g->right = n;
            else
                g->left = n;

            n->parent = g;
            //return p
        }
        p->parent = n;
        p->left =n->right;         
        if(n->right!=nullptr)
            n->right->parent = p;
        n->right = p;

        p->color = Color::Red;
        n->color = Color::Black;
        return p;
    }
   
    node* leftrightRotation(node* n,node* c)
    {
        c->parent = n->parent;
        n->parent->left = c;
        c->left = n;
        n->parent = c;
        n->right=c->right; // is null
        return c;
    }

    node* rightleftRotation(node* n,node* c)
    {
        c->parent =n->parent;
        n->parent->right = c;
        c->right = n;
        n->parent = c;
        n->left=c->left; // which is null
        return c;
    }
    
    //for delete fixup
    void leftRotation(node* n, node* c)
    {
        node* p = n->parent;
        if(p!=root)
        {
            node* g = p->parent;
            if(g->val < p->val)
                g->right = n;
            else
                g->left = n;

            n->parent = g;

        }
        if(p==root)
            root=n;
        p->parent = n;
        p->right =n->left;
        if(n->left!=nullptr)
            n->left->parent = p;
        n->left = p;
    }
    void rightRotation(node* n,node* c)
    {
        node* p = n->parent;
        if(p!=root)
        {
            node* g = p->parent;
            if(g->val < p->val)
                g->right = n;
            else
                g->left = n;

            n->parent = g;
            //return p
        }
        if(p==root)
            root=n;
        p->parent = n;
        p->left =n->right;         
        if(n->right!=nullptr)
            n->right->parent = p;
        n->right = p;
    }

    void print()
    {
        if (root==nullptr)
        {
            std::cout << "Empty Tree" << std::endl;
        }
        else
        {
            print(root,0);
        }
    }
    void print(node* n,int depth)
    {
        if(n->right!=nullptr)
        {
            print(n->right,depth+1);
        }
       
        for (size_t i = 0; i < depth; i++)
        {
            std::cout << "     " ;
        }
        std::cout << n->val << "(" << n->color_name() << ")" << std::endl;

        if(n->left!=nullptr)
        {
            print(n->left,depth+1);
        }
      
        
    }

    void deleteVal(int val)
    {
        
        node* n = find(val);
        if (n==nullptr)
        {
            std::cout << "Value not found" << std::endl;
            return;
        }
        // STEP 1
        node* x = nullptr;
        node* r = nullptr; // Replacement
        node* w = nullptr; // Sibling

       

        // Case 1
        if(n->left==nullptr && n->right==nullptr)
        {  
            // Just go to Step 2 
        }
        // Case 2a
        else if((n->left!=nullptr and n->right==nullptr))
        {
            r = n->left;
            x=r;

        
        }        
        // Case 2b
        else if((n->left==nullptr and n->right!=nullptr))
        {
            r = n->right;
            x= r;
        }
        // Case 3
        else // If node to be deleted has 2 children
        {
            r = getInorderSuccessor(n);
            x = r->right;
        }

        // SETTING r's Sibling first
        if(r==nullptr) // This is only possible from Case 1 of Step 1
        {
            if (n->val > n->parent->val) // n is on the right
            {
                // n->parent->right = x; // since n->right is already nullptr
                //or
                x = n->right;
                w = n->parent->left; 
            }
            else // n is on the left
            {
                // n->parent->left = x; 
                //or
                x = n->left;
                w = n->parent->right; 
            }
            
        }else
        {
            if(r->val > r->parent->val)
                w = r->parent->left;
            else
                w = r->parent->right;
        }
        //STEP 2
        if(n->color == Color::Red)
        {
        //Case 1
            if(r==nullptr or r->color==Color::Red)
            {
                
                node* p=nullptr;
                if(n==root) // but n is never Red if its root so this is obsolete
                {
                    root = nullptr;
                    size--;
                    delete n;
                }
                else
                {
                    if(r==nullptr)
                    { 
                        p = n->parent;
                        if(n->val > p->val) // n is in right
                        {
                            p->right = nullptr;
                        }else{
                            p->left = nullptr;
                        }
                        delete n;
                        size--;
                        return;
                    }else
                    {
                        p = n->parent;
                        if(r->val > r->parent->val) // r is on the right
                            r->parent->right = nullptr;
                        else
                            r->parent->left = nullptr;

                        r->parent = p;

                        if(n->val > p->val) // n is in right
                        {
                            p->right = r;
                            r->left = n->left;
                            r->right = n->right;
                            n->left->parent = r;
                            n->right->parent = r;
                        }else{
                            p->left = r;
                            r->left = n->left;
                            r->right = n->right;
                            n->left->parent = r;
                            n->right->parent = r;
                        }
                        delete n;
                        size--;
                        return;
                        
                    }
                }
                return;

            }
       
        //Case 2
            if(r!=nullptr and r->color==Color::Black)
            {
                
                node* p = n->parent;

                r->left = n->left;
                n->left->parent = r;
                if(r->parent != n)
                    r->right = n->right;
                r->parent = p;
                if(r->val > p->val) // r is on the right
                    p->right = r;
                else
                    p->left = r;

                n->left = nullptr;
                n->right = nullptr;
                r->color = Color::Red;
                
                delete(n);
                //Goto STEP 3
                delete_fixup(x,w);
                size--;
                return;
            }
        }
        //Case 3
        else if(n->color==Color::Black and (r!=nullptr and r->color==Color::Red))
        {
            node* g = nullptr;
            node* p = nullptr;
            if(n==root)
            {
                p = r->parent;
                if(r->val > p->val)//replacement is on the right
                {
                    p->right = nullptr;
                }
                else{
                    p->left = nullptr;
                }
                r->left = root->left;
                if(r->parent!=n)
                    r->right = n->right;
                delete root;
                root = r;
                size--;
            }
            else
            {
                g= n->parent;
                p = r->parent;

                r->left = n->left;
                if(r->parent!=n)
                    r->right = root->right;     

                if (g->val > n->val) // n is on the left
                {
                    g->left = r;
                }
                else
                {
                    g->right = r;
                }
                if(r->val > p->val) // r is on the right
                {
                    r->right = nullptr;
                }
                else{
                    r->left = nullptr;
                }
                r->parent = g;
                r->color = Color::Black;
                size--;
                return;
            }
        }

        //Case 4
        else if(n->color==Color::Black and (r==nullptr or r->color==Color::Black) and x!=root)
        {
            node* g = nullptr;
            node* p = nullptr;
            if(n==root and r==nullptr)
            {
            
                    root=nullptr;
                    delete n;
                    size--;
                    return;

            }
            else if(n!=root and r==nullptr) 
            {
                if(n->val < n->parent->val)
                    n->parent->left = nullptr;
                else
                    n->parent->right = nullptr;
                delete n;
                delete_fixup(x,w);
                size--;


            }
            else if(n==root and r!=nullptr)
            {
                p = r->parent;
                if(r->val > p->val)//replacement is on the right
                {
                    p->right = nullptr;
                }
                else{
                    p->left = nullptr;
                }
               
                r->left = root->left;
                root->left->parent = r;
                root->right->parent = r;
                if(r->parent!=n)
                {
                    r->parent->left = r->right;
                    r->right = n->right;
                }
                
                r->parent = nullptr;
                delete root;
                root = r;
                delete_fixup(x,w);
                size--;
                return;
                
            }
            else if (n!=root and r!=nullptr)
            {
                g= n->parent; // X
                p = r->parent; // 17

                r->left = n->left;
                n->left->parent = r;
                n->right->parent = r;

                if(r->parent!=n)
                {
                    r->parent->left = r->right;
                    r->right = n->right;    
                } 

                if (g->val > n->val) // n is on the left
                {
                    g->left = r;
                }
                else
                {
                    g->right = r;
                }
               
                
                r->parent = g;
                delete_fixup(x,w);
                size--;
                return;
            }
        }
        //Case 5
        else if(n->color==Color::Black and (r==nullptr or r->color==Color::Black) and x==root)
        {
            std::cout << "CASE 5 " << std::endl;
        }
        else{

            std::cout << " ELSE CASE " << std::endl;
        }
        
    }


    void delete_fixup(node* x,node* w)
    {
        //Case 0
        if(x!=nullptr and x->color==Color::Red)
        {
            x->color=Color::Black;
            return;
        }

        //Case 1
        if ((x==nullptr or x->color == Color::Black) and (w!=nullptr and w->color==Color::Red))
        {
            node *p = w->parent;
            w->color = Color::Black;
            p->color = Color::Red;
      
            if(w == p->right)  
            { 
                leftRotation(w,w->right);
            }else{
                rightRotation(w,w->left);
            }
            if (p->left==nullptr)
            {
                w = p->right; 
            }
            else
            {
                w = p->left;
            }
            delete_fixup(x,w);
            return;
            
        }
        //Case 2
        if ((x==nullptr or x->color == Color::Black) and 
                (w!=nullptr and (w->left==nullptr or w->left->color==Color::Black) and (w->right==nullptr or w->right->color==Color::Black)))
        {
            w->color = Color::Red;
            x = w->parent;
            if(x->color==Color::Red)
            {
                x->color=Color::Black;
            }
            else if(x->color==Color::Black and x==root)
            {
                // No need to do anything
            }
            else if(x->color==Color::Black and x!=root)
            {
                if(x==x->parent->left)
                {
                    w = x->parent->right;
                }else{
                    w = x->parent->left;
                }
                delete_fixup(x,w);
            }
            return;
        }
        // COMBINE CASE 3 AND 4
        if((x== nullptr or x->color == Color::Black) and (w!=nullptr and w->color == Color::Black) )
        {
            //Case 3
            node* p = w->parent;
            if (w->val > p->val and // w is on right and x is on left
                 (w->left!= nullptr and w->left->color == Color::Red and (w->right==nullptr or w->right->color == Color::Black )) ) 
            {
                w->left->color = Color::Black;
                w->color = Color::Red;
                rightleftRotation(w,w->left);
                w=p->right; // or w=w->parent
            }
            else if(w->val < p->val and // w is on left and x is on right
                (w->right!= nullptr and w->right->color == Color::Red and (w->left==nullptr or w->left->color == Color::Black )) ) 
            {
                w->right->color = Color::Black;
                w->color = Color::Red;
                leftrightRotation(w,w->right);
                w=p->left; // or w=w->parent
            }
            //Proceed to Case 4


            //Case 4
            if(w->val > w->parent->val  // x is on the left
               && (w->right!=nullptr and w->right->color == Color::Red)) 
            {
                w->color = w->parent->color;
                w->parent->color = Color::Black;
                w->right->color = Color::Black;
                //Rotate w's parent
                //Left rotation
                leftRotation(w,w->right);
            }
            else if (w->val < w->parent->val // x is on the right
                    and (w->left!=nullptr and w->left->color == Color::Red))
            {
                w->color = w->parent->color;
                w->parent->color = Color::Black;
                w->left->color = Color::Black;
                //Rotate w's parent
                //Right Rotation
                rightRotation(w,w->left);
            }
            
        }
        
    }

    node* getInorderSuccessor(node *n)
    {
        node* traverse = n->right;
        while(traverse->left!=nullptr)
        {
            traverse = traverse->left;
        }
        return traverse;
    }

    void deleteTree(node* n)
    {
        if(n==nullptr)
            return;
        
        deleteTree(n->left);
        deleteTree(n->right);
        delete n;
    }
public:
    node* root;
    size_t size = 0;
};

int main(int, char**){
    
    // Color x = Color::Red;

    RBTree test1(10);
    test1.insertNode(18);
    test1.insertNode(7);
    test1.insertNode(15);
    test1.insertNode(16);
    test1.insertNode(30);
    test1.insertNode(25);
    test1.insertNode(40);
    test1.insertNode(60);
    test1.insertNode(2);
    test1.insertNode(1);
    test1.insertNode(70);


    // node* t = test1.find(21);
    // std::cout << "Parent of 21 is " << t->parent->val << std::endl;
    
    test1.print();
   
    std::cout << " \n" << std::endl;
    std::cout << " \n TESTING DELETE \n" << std::endl;
    
    //------------------------TESTING-DELETE------------------------
    // testing every possible cases based on this youtube tutorial and example: 
    // https://www.youtube.com/watch?v=eoQpRtMpA9I
    RBTree example1(13);
    example1.insertNode(8);
    example1.insertNode(17);
    example1.insertNode(1);
    example1.insertNode(11);
    example1.insertNode(15);
    example1.insertNode(25);
    example1.insertNode(6);
    example1.insertNode(22);
    example1.insertNode(27);
    
    
    std::cout << " \n" << std::endl;
    std::cout << " Example 1" << std::endl;
    std::cout << " \n" << std::endl;
    example1.print();
    std::cout << " \n Deleting 6" << std::endl;
    example1.deleteVal(6);
    std::cout << " \n" << std::endl;
    example1.print();
    std::cout << " \n END OF PROGRAM \n" << std::endl;


    // EXAMPLE 2 
    RBTree example2(13);
    example2.insertNode(8);
    example2.insertNode(17);
    example2.insertNode(1);
    example2.insertNode(11);
    example2.insertNode(15);
    example2.insertNode(25);
    example2.insertNode(6);
    example2.insertNode(22);
    example2.insertNode(27);
    
    
    std::cout << " \n" << std::endl;
    std::cout << " Example 2" << std::endl;
    std::cout << " \n" << std::endl;
    example2.print();
    std::cout << " \n Deleting 1" << std::endl;
    example2.deleteVal(1);
    std::cout << " \n" << std::endl;
    example2.print();


    // EXAMPLE 3 
    RBTree example3(13);
    example3.insertNode(8);
    example3.insertNode(17);
    example3.insertNode(1);
    example3.insertNode(11);
    example3.insertNode(15);
    example3.insertNode(25);
    example3.insertNode(6);
    example3.insertNode(22);
    example3.insertNode(27);
    
    
    std::cout << " \n" << std::endl;
    std::cout << " Example 3" << std::endl;
    std::cout << " \n" << std::endl;
    example3.print();
    std::cout << " \n Deleting 17" << std::endl;
    example3.deleteVal(17);
    std::cout << " \n" << std::endl;
    example3.print();


    // EXAMPLE 4 
    RBTree example4(13);
    example4.insertNode(8);
    example4.insertNode(17);
    example4.insertNode(1);
    example4.insertNode(11);
    example4.insertNode(15);
    example4.insertNode(25);
    example4.insertNode(6);
    example4.insertNode(22);
    example4.insertNode(27);
    
    
    std::cout << " \n" << std::endl;
    std::cout << " Example 4" << std::endl;
    std::cout << " \n" << std::endl;
    example4.print();
    std::cout << " \n Deleting 25" << std::endl;
    example4.deleteVal(25);
    std::cout << " \n" << std::endl;
    example4.print();


    // EXAMPLE 5 
    RBTree example5(7);
    example5.insertNode(3);
    example5.insertNode(18);
    example5.insertNode(10);
    example5.insertNode(22);
    example5.insertNode(8);
    example5.insertNode(11);
    example5.insertNode(26);
    
    
    std::cout << " \n" << std::endl;
    std::cout << " Example 5" << std::endl;
    std::cout << " \n" << std::endl;
    example5.print();
    std::cout << " \n Deleting 18" << std::endl;
    example5.deleteVal(18);
    std::cout << " \n" << std::endl;
    example5.print();


    // EXAMPLE 6 
    RBTree example6(5);
    example6.insertNode(2);
    example6.insertNode(8);
    example6.insertNode(1);
    example6.insertNode(4);
    example6.insertNode(7);
    example6.insertNode(9);

    example6.find(2)->color = Color::Red;
    example6.find(1)->color = Color::Black;
    example6.find(4)->color = Color::Black;
    
    
    std::cout << " \n" << std::endl;
    std::cout << " Example 6" << std::endl;
    std::cout << " \n" << std::endl;
    example6.print();
    std::cout << " \n Deleting 2" << std::endl;
    example6.deleteVal(2);
    std::cout << " \n" << std::endl;
    example6.print();


    // EXAMPLE 7 
    RBTree example7(13);
    example7.insertNode(17);
    example7.insertNode(8);
    example7.insertNode(1);
    example7.insertNode(11);
    example7.insertNode(15);
    example7.insertNode(25);
    example7.insertNode(6);
    example7.insertNode(22);
    example7.insertNode(27);

    
    std::cout << " \n" << std::endl;
    std::cout << " Example 7" << std::endl;
    std::cout << " \n" << std::endl;
    example7.print();
    std::cout << " \n Deleting 13" << std::endl;
    example7.deleteVal(13);
    std::cout << " \n" << std::endl;
    example7.print();



    // EXAMPLE 8 
    RBTree example8(13);
    example8.insertNode(17);
    example8.insertNode(8);
    example8.insertNode(1);
    example8.insertNode(11);
    example8.insertNode(15);
    example8.insertNode(25);
    example8.insertNode(6);
    example8.insertNode(22);
    example8.insertNode(27);

    
    std::cout << " \n" << std::endl;
    std::cout << " Example 8" << std::endl;
    std::cout << " \n" << std::endl;
    example8.print();
    std::cout << " \n Deleting 8" << std::endl;
    example8.deleteVal(8);
    std::cout << " \n" << std::endl;
    example8.print();


    // EXAMPLE 9 
    RBTree example9(7);
    example9.insertNode(3);
    example9.insertNode(18);
    example9.insertNode(10);
    example9.insertNode(22);
    example9.insertNode(8);
    example9.insertNode(11);
    example9.insertNode(26);

    example9.find(18)->val = 11;
    example9.find(11)->val = 18;

    
    std::cout << " \n" << std::endl;
    std::cout << " Example 9" << std::endl;
    std::cout << " \n" << std::endl;
    example9.print();
    std::cout << " \n Deleting 3" << std::endl;
    example9.deleteVal(3);
    std::cout << " \n" << std::endl;
    example9.print();




    // EXAMPLE 10 
    RBTree example10(13);
    example10.insertNode(17);
    example10.insertNode(8);
    example10.insertNode(1);
    example10.insertNode(11);
    example10.insertNode(15);
    example10.insertNode(25);
    example10.insertNode(6);
    example10.insertNode(22);
    example10.insertNode(27);
    
    std::cout << " \n" << std::endl;
    std::cout << " EXAMPLE 10 - INSERTED ALL ELEMENTS \n" << std::endl;
    std::cout << "Current Size : " << example10.size  << std::endl;
    
    
    std::cout << " \n" << std::endl;
    std::cout << " Example 10" << std::endl;
    std::cout << " \n" << std::endl;
    example10.print();
    std::cout << " \n Deleting 10" << std::endl;
    example10.deleteVal(11);
    std::cout << " \n" << std::endl;
    example10.print();
    std::cout << "Current Size : " << example10.size  << std::endl;
    
    std::cout << " \n \n" << std::endl;
    std::cout << " \n Deleting 15" << std::endl;
    example10.deleteVal(15);
    std::cout << " \n" << std::endl;
    example10.print();
    std::cout << "Current Size : " << example10.size  << std::endl;
    
    std::cout << " \n \n" << std::endl;
    std::cout << " \n Deleting 1" << std::endl;
    example10.deleteVal(1);
    std::cout << " \n" << std::endl;
    example10.print();
    std::cout << "Current Size : " << example10.size  << std::endl;
    
    std::cout << " \n \n" << std::endl;
    std::cout << " \n Deleting 13" << std::endl;
    example10.deleteVal(13);
    std::cout << " \n" << std::endl;
    example10.print();
    std::cout << "Current Size : " << example10.size  << std::endl;
    
    std::cout << " \n \n" << std::endl;
    std::cout << " \n Deleting 25" << std::endl;
    example10.deleteVal(25);
    std::cout << " \n" << std::endl;
    example10.print();
    std::cout << "Current Size : " << example10.size  << std::endl;
    
    std::cout << " \n \n" << std::endl;
    std::cout << " \n Deleting 6" << std::endl;
    example10.deleteVal(6);
    std::cout << " \n" << std::endl;
    example10.print();
    std::cout << "Current Size : " << example10.size  << std::endl;
    
    std::cout << " \n \n" << std::endl;
    std::cout << " \n Deleting 27" << std::endl;
    example10.deleteVal(27);
    std::cout << " \n" << std::endl;
    example10.print();
    std::cout << "Current Size : " << example10.size  << std::endl;
    
    std::cout << " \n \n" << std::endl;
    std::cout << " \n Deleting 22" << std::endl;
    example10.deleteVal(22);
    std::cout << " \n" << std::endl;
    example10.print();
    std::cout << "Current Size : " << example10.size  << std::endl;
    
    std::cout << " \n \n" << std::endl;
    std::cout << " \n Deleting 17" << std::endl;
    example10.deleteVal(17);
    std::cout << " \n" << std::endl;
    example10.print();
    std::cout << "Current Size : " << example10.size  << std::endl;
    
    std::cout << " \n \n" << std::endl;
    std::cout << " \n Deleting 8" << std::endl;
    example10.deleteVal(8);
    std::cout << " \n" << std::endl;
    example10.print();
    std::cout << "Current Size : " << example10.size  << std::endl;
    
    std::cout << " \n END OF PROGRAM \n" << std::endl;
    return 0;
}
