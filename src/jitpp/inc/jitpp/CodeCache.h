#ifndef REMU_JITPP_CODECACHE_H
#define REMU_JITPP_CODECACHE_H

#include "jitpp/ACState.h"
#include "jitpp/CodePage.h"

namespace remu { namespace jitpp {

template<int WAYS, int ITEMS, size_t ITEM_SIZE, 
         class RandomStrategy, 
         class EvictionPolicy>
class CodeCache : public RandomStrategy
                , public EvictionPolicy
{
	using RandomStrategy::get_rand_way;
	using EvictionPolicy::on_item_evicted;

public:
	static const int SETS = ITEMS / WAYS;

	CodePage<ITEM_SIZE>* getPageForFarPointer( ACFarPointer &far_pointer ) {
		auto page = lookup( far_pointer );

		if( page ) {
			return page;
		}

		const int set = calcSet( far_pointer );
		const int way = findReplacementWay( set );

		if( valid[set][way] ) {
			on_item_evicted( &cache[set][way] );
		}
		else {
			valid[set][way] = true;
		}

		cache[set][way] = page = new CodePage<ITEM_SIZE>( far_pointer );

		return page;
	}

private:
	constexpr int calcSet( const ACFarPointer &far_pointer ) {
		return far_pointer.program_counter / SETS / ITEM_SIZE;
	}

	int findReplacementWay( int set ) const {
		for( int ii = 0; ii < WAYS; ii++ ) {
			if( !valid[set][ii] ) {
				return ii;
			}
		}

		return get_rand_way( WAYS );
	}

	CodePage<ITEM_SIZE>* lookup( ACFarPointer &far_pointer ) {
		const int set = calcSet( far_pointer );
		for( int ii = 0; ii < WAYS; ii++ ) {
			if( valid[set][ii] && tags[set][ii] == far_pointer ) {
				return cache[set][ii];
			}
		}
		return nullptr;
	}

	ACFarPointer tags[SETS][WAYS];
	bool valid[SETS][WAYS];
	CodePage<ITEM_SIZE>* cache[SETS][WAYS];
};

}} /*namespace remu::jitpp*/

#endif /*REMU_JITPP_CODECACHE_H*/

