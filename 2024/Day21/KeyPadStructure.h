#include <bits/stdc++.h>
using namespace std;

// Head file initialization use static const to avoid CMKAE warnig
static const unordered_map<char, vector<pair<char, char>>> NUMERIC_KEYPAD{

    {'A',{
                    {'3','^'},
            {'0','<'}
        }
    },

    {'0',
        {
                    {'2','^'},
                            {'A','>'}
        }
    },

    {'1',
        {
                    {'4','^'},
                            {'2','>'}
        }
    },
    {'2',
        {
                    {'5','^'},
            {'1','<'},
                            {'3','>'},
                    {'0','v'}
        }
    },
    {'3',
        {
                    {'6','^'},
            {'2','<'},
                    {'A','v'},
        }
    },
    {'4',
        {
                    {'7','^'},
                            {'5','>'},
                    {'1','v'},
        }
    },
    {'5',
        {
                    {'8','^'},
            {'4','<'},
                            {'6','>'},
                    {'2','v'},
        }
    },
    {'6',
        {
                    {'9','^'},
            {'5','<'},
                    {'3','v'},
        }
    },
    {'7',
        {
                            {'8','>'},
                    {'4','v'},
        }
    },
    {'8',
        {
            {'7','<'},
                            {'9','>'},
                    {'5','v'},
        }
    },
    {'9',
        {
            {'8','<'},
                    {'6','v'},
        }
    },
};


static const unordered_map<char, vector<pair<char, char>>> DIRECTIONAL_KEYPAD{
        {'A',
            {
                {'^','<'},
                        {'>','v'},
            }
        },
        {'^',
            {
                        {'A','>'},
                {'v','v'},
            }
        },
        {'v',
            {
                        {'^','^'},
                {'<','<'},
                                {'>','>'},
            }
        },
        {'<',
            {
                                {'v','>'},
            }
        },
        {'>',
            {
                        {'A','^'},
                {'v','<'},
            }
        }
};
