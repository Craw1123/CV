#!/usr/bin/env python
# coding: utf-8

# In[1]:



class Component():

    def operation(self) -> str:
        pass


class ConcreteComponent(Component):

    def operation(self) -> str:
        return "ConcreteComponent"


class Decorator(Component):

    _component: Component = None

    def __init__(self, component: Component) -> None:
        self._component = component

    @property
    def component(self) -> Component:

        return self._component

    def operation(self) -> str:
        return self._component.operation()


class ConcreteDecoratorA(Decorator):

    def operation(self) -> str:
        return f"|{self.component.operation()}|"


class ConcreteDecoratorB(Decorator):

    def operation(self) -> str:
        return f"{self.component.operation()}logo"
    
class ConcreteDecoratorC(Decorator):

    def operation(self) -> str:
        return f"{self.component.operation()}logo"

def client_code(component: Component) -> None:

    print(f"RESULT: {component.operation()}", end="")



if __name__ == "__main__":
    a = "Some text to decorate"
    simple = ConcreteComponent()
    print("I've got a simple component:")
    client_code(simple)
    print("\n")
    decorator1 = ConcreteDecoratorA(simple)
    decorator2 = ConcreteDecoratorB(decorator1)
    decorator3 = ConcreteDecoratorC(decorator2)
    print("Now I've got a decorated component:")
    client_code(decorator3)


# In[5]:





# In[ ]:




