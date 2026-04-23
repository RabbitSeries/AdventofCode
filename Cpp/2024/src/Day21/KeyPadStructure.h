#include <unordered_map>
#include <map>
#include <vector>

// Head file initialization use inline to share one copy
inline const std::unordered_map<char, std::vector<std::pair<char, char>>> NUMERIC_KEYPAD{

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


inline const std::unordered_map<char, std::vector<std::pair<char, char>>> DIRECTIONAL_KEYPAD{
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
