#include "bits/stdc++.h"
#define BUFFER_SIZE 1024
using namespace std;
typedef long long ll;
typedef struct dualLinkList {
    dualLinkList() {};
    dualLinkList( ll _data ) : data( _data ) {}
    dualLinkList* pre = NULL;
    dualLinkList* next = NULL;
    ll data;
}*Node;

void blink( Node& head, Node& tail ) {
    Node p = head;
    while( p && p != tail ) {
        p = p->next;
        // Rule 1
        stringstream ss;
        ss << p->data;
        if( p->data == 0 ) {
            p->data = 1;
        }
        else if( ss.str().size() % 2 == 0 ) {
            // Rule 2
            int middle = ss.str().size() / 2;
            string left = ss.str().substr( 0, middle ), right = ss.str().substr( middle );
            p->data = stoll( right );
            Node q = new dualLinkList( stoll( left ) );
            q->pre = p->pre;
            q->pre->next = q;
            p->pre = q;
            q->next = p;
        }
        else {
            // Rule 3
            p->data *= 2024;
        }
    }
    return;
}

int main() {
    // vector<int> initialArr;
    // Node head = (Node)malloc(sizeof(dualLinkList));
    Node head = new dualLinkList;
    Node tail = head;
    FILE* input = fopen( "input.txt", "r" );
    char linebuf[BUFFER_SIZE + 1] = "\0";
    while( !feof( input ) && fgets( linebuf, BUFFER_SIZE, input ) ) {
        stringstream line( linebuf );
        int tmp = 0;
        while( line >> tmp ) {
            tail->next = new dualLinkList( tmp );
            tail->next->pre = tail;
            tail = tail->next;
        }
    }
    int twinkle = 25;
    for( int i = 0; i < twinkle; i++ ) {
        cout << "Times: " << i << endl;
        blink( head, tail );
    }
    Node p = head;
    ll res = 0;
    while( p && p != tail ) {
        res++;
        p = p->next;
    }
    cout << res << endl;
    return 0;
}
