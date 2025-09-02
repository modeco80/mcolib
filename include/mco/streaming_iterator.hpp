#pragma once

#include <concepts>
#include <iterator>
#include <optional>
#include <vector>

namespace mco {

	/// Concept of lazy iterators.
	/// These act pretty much analoguously to Rust iterators.
	/// however, C++ doesn't have sugar like Rust does to use them.
	/// This makes them a bit uglier, but whatever.
	template <class T>
	concept LazyIteratorLike = requires(T iter) {
		typename T::Type;
		// .next() must return optional<Type>. std::nullopt marks the end of the
		// iterator.
		{ iter.next() } -> std::convertible_to<std::optional<typename T::Type>>;
	};

	/// Functions like C++ range for, or Rust "for r in ..."
	template <class Func>
	auto forEach(LazyIteratorLike auto& iter, Func&& func) {
		while(true) {
			auto item = iter.next();
			if(!item.has_value())
				break;
			func(item.value());
		}
	}

	/// Functions like Rust `(...).collect::<Vec<_>>()`.
	template <LazyIteratorLike Iter>
	auto collect(Iter& iter) -> std::vector<typename Iter::Type> {
		std::vector<typename Iter::Type> items;
		forEach(iter, [&items](auto& item) {
			items.push_back(item);
		});
		return items;
	}

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