#pragma once

#include <mco/lazy_iterator.hpp>

namespace mco {

	/// Returns an iterator which lazily evaluates to a integer sequence, jumping by
	/// [increment] each time.
	template <class T>
	auto iota(T start, T end, T increment = 1) {
		struct IotaImpl {
			using Type = T;

			constexpr IotaImpl(T start, T increment, T end)
				: rangeStart(start), rangeIncrement(increment), rangeEnd(end) {
				current = rangeStart;
			}

			std::optional<Type> next() {
				if(current + rangeIncrement >= rangeEnd)
					return std::nullopt;
				auto prev = current;
				current += rangeIncrement;
				return prev;
			}

		   private:
			T rangeStart;
			T rangeIncrement;
			T rangeEnd;
			T current;
		};
		static_assert(LazyIteratorLike<IotaImpl>, "IotaImpl needs to be updated");
		return IotaImpl(start, increment, end);
	}

	/// Returns a lazy iterator which filters items via a user-provided map function.
	template <LazyIteratorLike Iter, class MapFunc>
	auto map(Iter& iter, MapFunc&& mapFunc) {
		struct MapImpl {
			using Type = typename Iter::Type;
			constexpr MapImpl(Iter& iter, MapFunc mapFunc)
				: underlyingIter(iter), map(static_cast<MapFunc&&>(mapFunc)) {
			}
			std::optional<Type> next() {
				// Filter through yielded values (discarding them) until the iterator provides
				// a value which passes the user-provided map function. Once it does, we return it.
				while(true) {
					if(auto item = underlyingIter.next(); item.has_value()) {
						if(map(item.value())) {
							// The value the iterator yielded passed the map function.
							return item;
						} else {
							// The value the iterator gave us did not pass the map function.
							// Discard it and yield another one.
							continue;
						}
					} else {
						// The iterator ended.
						break;
					}
				}
				return std::nullopt;
			}

		   private:
			Iter& underlyingIter;
			MapFunc map;
		};
		static_assert(LazyIteratorLike<MapImpl>, "MapIteratorImpl needs to be updated");
		return MapImpl(iter, mapFunc);
	}

} // namespace mco