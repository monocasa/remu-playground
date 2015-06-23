#ifndef REMU_JITPP_CODECACHE_H
#define REMU_JITPP_CODECACHE_H

#include "jitpp/ACState.h"
#include "jitpp/CodePage.h"

#include <cstdio>

namespace remu { namespace jitpp {

template<int WAYS, int ITEMS, size_t ITEM_SIZE, 
         class RandomStrategy, 
         class EvictionPolicy,
         class TranslatorFactory,
         class Translator,
         class HostPageSelectionStrategy>
class CodeCache : public RandomStrategy
                , public EvictionPolicy
                , private HostPageSelectionStrategy
{
	using RandomStrategy::get_rand_way;
	using EvictionPolicy::on_item_evicted;
	using HostPageSelectionStrategy::get_host_page;

public:
	static const int SETS = ITEMS / WAYS;

	CodePage<ITEM_SIZE, Translator>* getPageForFarPointer( ACFarPointer &far_pointer ) {
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

		cache[set][way] = page = new CodePage<ITEM_SIZE, Translator>( far_pointer,
		                                      translator_factory.translator_for_code_segment(far_pointer.code_segment),
		                                      get_host_page(far_pointer) );
		tags[set][way] = getPageFarPointer( far_pointer );

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

	CodePage<ITEM_SIZE, Translator>* lookup( const ACFarPointer &far_pointer ) const {
		const auto page_far_pointer = getPageFarPointer( far_pointer );
		const int set = calcSet( page_far_pointer );

		for( int ii = 0; ii < WAYS; ii++ ) {
			if( valid[set][ii] && tags[set][ii] == page_far_pointer ) {
				return cache[set][ii];
			}
		}
		return nullptr;
	}

	ACFarPointer getPageFarPointer( const ACFarPointer &non_aligned ) const {
		ACFarPointer page_far_pointer = { 
			(non_aligned.program_counter / ITEM_SIZE) * ITEM_SIZE,
			non_aligned.code_segment
		};

		return page_far_pointer;
	}

	ACFarPointer tags[SETS][WAYS];
	bool valid[SETS][WAYS];
	CodePage<ITEM_SIZE, Translator>* cache[SETS][WAYS];
	TranslatorFactory translator_factory;
};

}} /*namespace remu::jitpp*/

#endif /*REMU_JITPP_CODECACHE_H*/

