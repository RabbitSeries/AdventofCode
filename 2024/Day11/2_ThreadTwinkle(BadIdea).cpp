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

void ThreadBlink( Node head, Node tail, int times, vector<ll>& partialCnt, int threadId ) {
    for( int i = 0; i < times; i++ ) {
        if( i > 40 )
            cout << "Thread " << threadId << " at times: " << i << endl;
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
    }
    Node p = head;
    ll res = 0;
    while( p && p != tail ) {
        res++;
        p = p->next;
    }
    partialCnt[threadId] = res;
    cout << "Thread ID " << threadId << " res : " << res << endl;
    return;
}

pair<Node, Node> createList( ll data ) {
    Node head = new dualLinkList;
    Node tail = head;
    tail->next = new dualLinkList( data );
    tail->next->pre = tail;
    tail = tail->next;
    return pair<Node, Node>( head, tail );
}

int main() {
    // vector<int> initialArr;
    // Node head = (Node)malloc(sizeof(dualLinkList));
    vector<pair<Node, Node>> headList;
    FILE* input = fopen( "input.txt", "r" );
    char linebuf[BUFFER_SIZE + 1] = "\0";
    int stoneCnt = 0;
    while( !feof( input ) && fgets( linebuf, BUFFER_SIZE, input ) ) {
        stringstream line( linebuf );
        int tmp = 0;
        while( line >> tmp ) {
            stoneCnt++;
            headList.push_back( createList( tmp ) );
        }
    }
    int twinkle = 75;
    vector<thread> threadList;
    vector<ll> partialCnt( stoneCnt, 0 );
    for( int i = 0; i < stoneCnt; i++ ) {
        threadList.push_back( thread( ThreadBlink, headList[i].first, headList[i].second, twinkle, ref( partialCnt ), i ) );
    }
    for( auto& t : threadList ) {
        t.join();
    }
    // stoneCnt = 0;
    // vector<pair<Node, Node>> newheadList;
    // for( auto partList : headList ) {
    //     Node p = partList.first;
    //     while( p && p != partList.second ) {
    //         p = p->next;
    //         stoneCnt++;
    //         newheadList.push_back( createList( p->data ) );
    //     }
    // }
    // int twinkleLeft = 75 - twinkle;
    // partialCnt = vector<ll>( stoneCnt, 0 );
    // threadList.clear();
    // for( int i = 0; i < stoneCnt; i++ ) {
    //     threadList.push_back( thread( ThreadBlink, newheadList[i].first, newheadList[i].second, twinkleLeft, ref( partialCnt ), i ) );
    // }
    // for( auto& t : threadList ) {
    //     t.join();
    // }
    ll res = 0;
    for( auto partialRes : partialCnt ) {
        res += partialRes;
    }
    FILE* output = fopen( "output.txt", "w" );
    fprintf( output, "%ld", res );
    cout << "Final res :" << res;
    return 0;
}