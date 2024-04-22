#ifndef DLIB_LZ77_BUFFER_KERNEL_ABSTRACT_
#define DLIB_LZ77_BUFFER_KERNEL_ABSTRACT_

#include "../algs.h"

namespace dlib
{
    class lz77_buffer_kernel_abstract
    {
    public:
        lz77_buffer_kernel_abstract(
            unsigned long total_limit,
            unsigned long lookahead_limit
        );
        /*!
            requires
                - 6 < total_limit < 32
                - 15 < lookahead_limit <= 2^(total_limit-2)
            ensures                
                - #*this is properly initialized
                - #get_history_buffer_limit() == 2^total_limit  - lookahead_limit
                - #get_lookahead_buffer_limit() == lookahead_limit
            throws
                - std::bad_alloc
        !*/

        virtual ~lz77_buffer_kernel_abstract(
        );
        /*!
            ensures
                - any resources associated with *this have been released
        !*/

        void clear(
        );
        /*!
            ensures
                - #*this has its initial value
            throws
                - std::bad_alloc
                    if this exception is thrown then #*this is unusable 
                    until clear() is called and succeeds
        !*/

        void shift_buffers(
            unsigned long N
        );
        /*!
            requires
                - N <= get_lookahead_buffer_size()
            ensures
                - #get_lookahead_buffer_size() == get_lookahead_buffer_size() - N
                - #get_history_buffer_size() >= N
                - #get_history_buffer_size() <= get_history_buffer_size()+N
                - #get_history_buffer_size() <= get_history_buffer_limit()
                - for all i where 0 <= i < N:
                    #history_buffer(N-1-i) == lookahead_buffer(i)
                - for all i where 0 <= i < #get_history_buffer_size()-N:
                    #history_buffer(N+i) == history_buffer(i)
                - for all i where 0 <= i < #get_lookahead_buffer_size()
                    #lookahead_buffer(i) == lookahead_buffer(N+i)            
        !*/

        void add(
            unsigned char symbol
        );
        /*!
            ensures
                - if (get_lookahead_buffer_size() == get_lookahead_buffer_limit()) then
                    - performs shift_buffers(1)
                    - #lookahead_buffer(get_lookahead_buffer_limit()-1) == symbol
                    - #get_lookahead_buffer_size() == get_lookahead_buffer_size()
                - else
                    - #lookahead_buffer(get_lookahead_buffer_size()) == symbol
                    - #get_lookahead_buffer_size() == get_lookahead_buffer_size() + 1                                    
            throws
                - std::bad_alloc
                    if this exception is thrown then #*this is unusable 
                    until clear() is called and succeeds
        !*/

        void find_match(
            unsigned long& index,
            unsigned long& length,
            unsigned long min_match_length
        );
        /*!
            ensures
                - if (#length != 0) then
                    - #length >= min_match_length
                    - for all i where 0 <= i < #length:
                      history_buffer(#index-i) == lookahead_buffer(i)
                    - performs shift_buffers(#length)
            throws
                - std::bad_alloc
                    if this exception is thrown then #*this is unusable 
                    until clear() is called and succeeds
        !*/

        unsigned long get_history_buffer_limit(
        ) const;
        /*!
            ensures
                - returns the max number of symbols that can fit in the history buffer
        !*/

        unsigned long get_lookahead_buffer_limit(
        ) const;
        /*!
            ensures
                - returns the max number of symbols that can fit in the lookahead buffer
        !*/

        unsigned long get_history_buffer_size(
        ) const;
        /*!
            ensures
                - returns the number of symbols currently in the history buffer
        !*/

        unsigned long get_lookahead_buffer_size(
        ) const;
        /*!
            ensures
                - returns the number of symbols currently in the lookahead buffer
        !*/

        unsigned char lookahead_buffer(
            unsigned long index
        ) const;
        /*!
            requires
                - index < get_lookahead_buffer_size()
            ensures
                - returns the symbol in the lookahead buffer at location index
        !*/

        unsigned char history_buffer(
            unsigned long index
        ) const;
        /*!
            requires
                - index < get_history_buffer_size()
            ensures
                - returns the symbol in the history buffer at location index
        !*/

    private:
        // restricted functions
        lz77_buffer_kernel_abstract(lz77_buffer_kernel_abstract&);        // copy constructor
        lz77_buffer_kernel_abstract& operator=(lz77_buffer_kernel_abstract&);    // assignment operator
    };      
}

#endif // DLIB_LZ77_BUFFER_KERNEL_ABSTRACT_