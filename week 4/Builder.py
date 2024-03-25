import json
class Builder():
    def __init__(self, F_base, params_name, params, variables_name, initial_state, dt, T, dx, X):
        self.JS={'params_name': params_name.split(), 'params': params, 'variables_name': variables_name.split(), 'initial_state': initial_state, 'dt': dt, 'T': T, 'dx': dx, 'X': X}
        with open(F_base+'.json', 'w') as js:
            json.dump(self.JS, js)