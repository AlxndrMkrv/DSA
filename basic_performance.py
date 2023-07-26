from timeit import timeit


class Basic:
    def __init__(self, max_value: int = 1000, steps_number: int = 1000):
        self.N = [int(x) for x in range(1, max_value,
                                        round(max_value/steps_number))]

    @staticmethod
    def __init_container(TContainer, n: int):
        return TContainer(range(n))

    def measure_append_time(self, TContainer, replay: int = 10):
        return [timeit(f"[obj.append(0) for i in range({n})]", number=replay,
                       globals={'obj': TContainer()})
                for n in self.N]

    def measure_get_time(self, TContainer, replay: int = 10):
        return [timeit(f"obj[{int(n/2)}]", number=replay,
                       globals={'obj': self.__init_container(TContainer, n)})
                for n in self.N]

    def measure_size_time(self, TContainer, replay: int = 10):
        return [timeit("len(obj)", number=replay,
                       globals={'obj': self.__init_container(TContainer, n)})
                for n in self.N]

    def measure_insert_time(self, TContainer, replay: int = 10):
        return [timeit(f"obj.insert({int(n/2)}, 0)", number=replay,
                       globals={'obj': self.__init_container(TContainer, n)})
                for n in self.N]

    def measure_pop_time(self, TContainer, replay: int = 10):
        return [timeit("obj.pop(int(n/2))", "obj = TContainer(range(n))",
                       number=replay,
                       globals={'TContainer': TContainer, 'n': n})
                for n in self.N]

    def measure_initialization(self, TContainer, replay: int = 10):
        return [timeit("TContainer(range(n))", number=replay,
                       globals={"TContainer": TContainer, 'n': n})
                for n in self.N]

