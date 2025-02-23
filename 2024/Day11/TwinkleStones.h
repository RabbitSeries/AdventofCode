#include "bits/stdc++.h"
using namespace std;

class TwinkleStones {
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
    }

    map<ll, ll> nextBlink( const map<ll, ll> TwinkleList ) {
        map<ll, ll> nextList;
        for( auto stone : TwinkleList ) {
            stringstream ss;
            ss << stone.first;
            if( stone.first == 0 ) {
                nextList[1] += stone.second;
            }
            else if( ss.str().size() % 2 == 0 ) {
                // Rule 2
                int middle = ss.str().size() / 2;
                string left = ss.str().substr( 0, middle ), right = ss.str().substr( middle );
                nextList[stoll( left )] += stone.second;
                nextList[stoll( right )] += stone.second;
            }
            else {
                // Rule 3
                nextList[stone.first * 2024] += stone.second;
            }
        }
        return nextList;
    }
public:
    void Solution1() {
        Node head = new dualLinkList;
        Node tail = head;
        ifstream input( "Day11/input.txt" );
        string linebuf;
        while( getline( input, linebuf ) ) {
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
            // cout << "Times: " << i << endl;
            blink( head, tail );
        }
        Node p = head;
        ll res = 0;
        while( p && p != tail ) {
            res++;
            p = p->next;
        }
        cout << "Solution 1: " << res << endl;
    }

    void Solution2() {
        map<ll, ll> TwinkleList;
        ifstream input( "Day11/input.txt" );
        string linebuf;
        while( getline( input, linebuf ) ) {
            stringstream line( linebuf );
            int tmp = 0;
            while( line >> tmp ) {
                TwinkleList[tmp]++;
            }
        }
        for( int twinkle = 0; twinkle < 75; twinkle++ ) {
            // cout << "Twinkle times: " << twinkle << "." << endl;
            TwinkleList = nextBlink( TwinkleList );
        }
        ll res = 0;
        for( auto partialRes : TwinkleList ) {
            res += partialRes.second;
        }
        cout << "Solution 2: " << res << endl;
    }

};