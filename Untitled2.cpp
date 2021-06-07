#include <bits/stdc++.h>

using namespace std;

struct node
{
    string data;
    node* left;
    node* right;
};

string s, PostFix;
stack<node*> _stack;

int global_length;

int _get_height(node *sr)
{
    if (!sr)
        return -1;
    return 1 + max(_get_height(sr->left), _get_height(sr->right));
}

void _display_subtree(node *q, char **output, int left, int top, int width)
{
    string str = q->data;

    int left_begin_shift = 1 - (str.length()-1)/2;
    for (size_t i = 0; i < str.length() && left + width/2 + i < global_length; i++)
        output[top][left + width/2 + left_begin_shift + i] = str[i];

    int branch_off_set = (width+3)/pow(2, 3);
    int mid = left + width/2;
    int left_mid = left + (width/2 - 1)/2;
    int right_mid = left + width/2 + 2 + (width/2 - 1)/2;

    if (q->left)
    {
        int branch_position = mid - branch_off_set + 1;
        for (int pos = mid + left_begin_shift - 2; pos > branch_position; pos--)
            output[top][pos] = '_';
        output[top+1][branch_position] = '/';
        for (int pos = branch_position-1; pos > left_mid + 2; pos--)
            output[top+1][pos] = '_';
        _display_subtree(q->left, output, left, top+2, width/2 - 1);
    }

    if (q->right)
    {
        int branch_position = mid + branch_off_set + 1;
        for (int pos = mid + left_begin_shift + str.length() + 1; pos < branch_position; pos++)
            output[top][pos] = '_';
        output[top+1][branch_position] = '\\';
        for (int pos = branch_position+1; pos < right_mid; pos++)
            output[top+1][pos] = '_';
        _display_subtree(q->right, output, left + width/2 + 2, top+2, width/2 - 1);
    }
}


void _display_tree(node *root)
{
    if (!root)
        return;

    int height = _get_height(root);
    int width_display = 4*pow(2, height) - 3;
    int height_display = 2 * height + 1;

    global_length = width_display + 4;
    char **output = new char *[height_display];

    for (int i = 0; i < height_display; i++)
    {
        output[i] = new char [width_display + 4];
        for (int j = 0; j <  width_display + 4; j++)
        {
            if (j == width_display + 3)
            output[i][j] = '\0';
            else
                output[i][j] = ' ';
        }
    }

    _display_subtree(root, output, 0, 0, width_display);

    for (int row = 0; row < height_display; row++)
        cout << output[row] << endl;
}

string xoacach(string st)//xoa dau cach
{
    int vitri = st.find(' ');
    while (vitri != -1)
    {
        st.erase(vitri,1);
        vitri = st.find(' ');
    }
    return st;
}

bool IsOperator(char c)//kiem tra co phai toan tu khong
{
    return(c == '+' || c == '-' || c == '*' || c == '/');
}

node* CreateTree(string PF)//tao cay
{
    string temp = "";
    for (int i = 0; i < PF.length(); i++)
    {
        if (PF[i] == ' ')
        {
            node* new_node = new node();
            new_node->data = temp;
            if (IsOperator(temp[0]))
            {
                new_node->right = _stack.top();
                _stack.pop();
                new_node->left = _stack.top();
                _stack.pop();
            }
            _stack.push(new_node);
            temp = "";
        }else
            temp = temp + PF[i];
    }
    node* t = _stack.top();
    _stack.pop();
    return(t);
}

int _getpriority(char c)//ham lay do uu tien
{
    switch (c)
    {
        case '(' : return 1;
        case '+' : return 2;
        case '-' : return 2;
        case '*' : return 3;
        case '/' : return 3;
        case ')' : return -1;
    }
    return (0);
}

string _Postfix(string st)//chuyen bieu thuc sang bieu thuc hau to
{
    stack<char> t;
    int n;
    string tmp = "";
    string Pf = "";
    n = st.length();
    for (int i = 0; i < n; i++)
    {
        switch (st[i])
        {
            case '(' : t.push(st[i]); break;
            case ')' : while (t.top() != '(')
                          {
                              Pf = Pf + " " + t.top();
                              t.pop();
                          }
                        t.pop();
                        break;
            default :
                if (_getpriority(st[i]) == 0)
                {
                    tmp = tmp + st[i];
                    if (i == n-1 || _getpriority(st[i+1]) != 0)
                    {
                        Pf = Pf + " " + tmp;
                        tmp = "";
                    }
                    continue;
                }
                if (t.empty())
                {
                    t.push(st[i]);
                    continue;
                }
                if (_getpriority(st[i]) > _getpriority(t.top())) t.push(st[i]);
                else
                {
                     while (!t.empty() && _getpriority(st[i]) <= _getpriority(t.top()))
                    {
                        Pf = Pf + " " + t.top();
                        t.pop();
                    }
                    t.push(st[i]);
                }
        }
    }

    while (!t.empty())
    {
        Pf = Pf + " " + t.top();
        t.pop();
    }
    Pf.erase(0,1);
    return Pf;
}

double Cal(node *root)//tinh gia tri bieu thuc
{
    if (root->left == NULL && root->right == NULL)
    {
        return stod(root->data);
    }
    double x = Cal(root->left);
    double y = Cal(root->right);
    switch (root->data[0])
    {
        case '+' : return x + y;
        case '-' : return x - y;
        case '*' : return x * y;
        case '/' : return x / y;
    }
}


int main()
{
    node* root;
    cout<<"Nhap bieu thuc"<< endl;
    getline(cin,s);
    //s = "(A-B)-C";
    //s = "A-(B-C)";
    s = xoacach(s);
    PostFix = _Postfix(s);
    PostFix = PostFix + " ";
    cout << "Bieu thuc hau to(Postfix) : "<<PostFix << endl;
    root = CreateTree(PostFix);//tao cay
    //_display_tree(root);//print cay
    cout << "Gia tri bieu thuc : ";
    cout <<fixed << setprecision(3) <<   Cal(root);//tinh bieu thuc
    return 0;
}
