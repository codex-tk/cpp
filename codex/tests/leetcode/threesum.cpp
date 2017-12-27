#include <gtest/gtest.h>
#include "../helper.hpp"
#include <vector>
namespace {
using namespace std;
class Solution {
public:
    vector<vector<int>> threeSum(vector<int>& nums) {
        vector<vector<int>> ans;
        if ( nums.size() < 3 ) 
            return ans;
        sort( nums.begin() , nums.end());
        for ( std::size_t i = 0 ; i < nums.size() - 2 ; ++i ) {
            if ( i != 0 && nums[i-1] == nums[i] ) 
                continue;

            int ans0 = nums[i];
            int target = -ans0;

            int lo = i + 1;
            int hi = nums.size() - 1;

            while ( lo < hi ) {
                int value = nums[lo] + nums[hi];
                if ( value == target ) {
                    ans.push_back( { ans0 , lo , hi });
                    while ( lo < hi && nums[lo] == nums[lo+1] ) ++lo;
                    while ( lo < hi && nums[hi] == nums[hi-1] ) --hi;
                    ++lo;--hi;
                } else if ( value < target ) {
                    ++lo;
                } else {
                    --hi;
                }
            }
        }
        return ans;
    }
};

}



TEST( threeSum , ans ) {
    Solution s;
    //s.threeSum(std::vector<int>{0,0,0,0});
}

void sugar( int sugar) {
    std::cout << sugar << " : ";
    int kg3 = 0;
    while ( sugar >= 0 ) {
        if ( sugar % 5 == 0 ) {
            std::cout << (sugar / 5 + kg3) << std::endl;
            return;
        }
        sugar -= 3;
        ++kg3;
    }
    std::cout << -1 << std::endl;
}


TEST( sugar , ans ) {

}
int word_count( const std::string& sentence ) {
    bool is_word_start = true;
    int word_count = 0;
    for ( auto ch : sentence ) {
        if ( is_word_start && ch != ' ' ) {
            ++word_count;
            is_word_start = false;
        }
        if ( ch == ' ' ) {
            is_word_start = true;
        }
    }
    return word_count;
}
TEST( word_count , ans ) {
    ASSERT_EQ( word_count( "The Curious Case of Benjamin Button" ) , 6 );
    ASSERT_EQ( word_count( "" ) , 0 );
    ASSERT_EQ( word_count( " The Curious Case of Benjamin Button" ) , 6 );
    ASSERT_EQ( word_count( " The Curious Case of Benjamin Button " ) , 6 );
}



int d( int v ) {
    int val = v;
    while ( v > 0 ) {
        val += (v % 10);
        v /= 10;
    }
    return val;
}

TEST( self_number , ans ) {
    std::vector<int> ans( 10001 , 0 );
    for ( int i = 1 ; i <= 10000 ; ++i ) {
        int val = d(i);
        if ( val <= 10000 )
            ans[val] = 1;
    }
    for ( int i = 1 ; i <= 10000 ; ++i ) {
        if ( ans[i] == 0 ) {
            //std::cout << i << std::endl;
        }
    }
}

long long numbers[1000001] = {0 ,};
long long prinum[10000] = {0 ,};
 
int sq0(long long min , long long max ){
    long long num;
    int prinum_num = 0;
    int count = 0;
    for (long long i = 2; i*i <= max; i++){
        long long x = min / (i*i);
        if (min % (i*i) != 0)
            x++;
            
        while (x*(i*i) <= max){
            numbers[x*(i*i) - min] = 1;
            x++;
        }
 
    }
    for (int i = 0; i <= max - min; i++){
        if (numbers[i] == 0)
            count++;
    }
    return count;
}


std::vector<int> nums( 1000001 , 0 );
int sq1(long long mi , long long mx ){
    long long i = 2 ;
    long long base = 0;
    while ( base <= mx ) {
        base = i * i; ++i;
        long long div = ( mi / base );
        if ( mi % base != 0 ) ++div;
        long long value = base * div;
        while( value <= mx ) {
            if ( value >= mi ) nums[ value - mi ] = 1;
            value += base;
        }
    }
    long long count = 0;
    for ( std::size_t i = 0 ; i <= mx - mi ; ++i ) {
        if ( nums[i] == 0 ) ++count;
    }
    return count;
}

TEST( sq , ans ) {
    sq0( 1000000000000 , 1000001000000 );
}
TEST( sq , ans1 ) {
     sq1( 1000000000000 , 1000001000000);
}

#include <algorithm>

class int_iter : public std::iterator<
    std::input_iterator_tag
    , int
    , int
    , const int*
    , int >{

public:
    explicit int_iter( const int v ) : _value(v){}
    int& operator*(){ return _value; }
    int_iter& operator++(void) { ++_value; return *this;}
    int_iter operator++(int){ int_iter ret(_value); ++(*this); return ret; }
    bool operator==( const int_iter& rhs ) const {
        return _value == rhs._value;
    }
    bool operator!=( const int_iter& rhs ) const {
        return _value != rhs._value;
    }
private:
    int _value;
};

TEST( bj , ans1 ) {
    int input = 32;
    std::copy(  int_iter(1) 
              , int_iter(input) 
              , std::ostream_iterator<int>(std::cout , "\n"));
}