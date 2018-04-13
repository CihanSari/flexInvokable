#include <iostream>
#include "../include/flexInvokable.hpp"

void main() {
    auto executeThat = [](csari::flexInvokable<void()> invokableFnc) {
        std::move(invokableFnc)();
    };
	csari::flexInvokable<void()> a = [mu = std::make_unique<int>(5)]{
		std::cout << *mu.get() << '\n';
	};
    executeThat(std::move(a));
}

