#pragma once

#include <concepts>
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
		// .next() must return optional<Type>. 
		// std::nullopt marks the end of the iteration.
		{ iter.next() } -> std::convertible_to<std::optional<typename T::Type>>;

		// FIXME: Should we support usage of coroutines too? Like,
		// should `co_await iter` return next (or throw if the iterator is finished?)
		// I'm not sure how this should look.
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

	
} // namespace mco