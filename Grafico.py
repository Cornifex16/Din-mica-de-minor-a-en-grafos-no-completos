import matplotlib.pyplot as plt
import csv
import math
import numpy as np

def guardar_archivo(nombre, p, esperanza, desviacion_estandar, varianza, minimo, maximo):
    with open(nombre + ".csv", "a", newline='') as archivo:
        campos = ['p', 'esperanza', 'desviacion estandar', 'varianza', 'minimo', 'maximo']
        writer = csv.DictWriter(archivo, fieldnames=campos)
        writer.writerow({'p':p, 'esperanza': esperanza, 'desviacion estandar': desviacion_estandar, 'varianza': varianza, "minimo": minimo, "maximo": maximo})
    pass

def crear_archivo(nombre):
    with open(nombre + ".csv", "w", newline='') as archivo:
        campos = ['p', 'esperanza', 'desviacion estandar', 'varianza', 'minimo', 'maximo']
        writer = csv.DictWriter(archivo, fieldnames=campos)
        writer.writeheader()

def crear_archivo_3d(nombre):
    with open(nombre + ".csv", "w", newline='') as archivo:
        campos = ['n', 'a', 'esperanza', 'desviacion estandar', 'varianza', 'minimo', 'maximo']
        writer = csv.DictWriter(archivo, fieldnames=campos)
        writer.writeheader()

def guardar_archivo_3d(nombre, n, a, esperanza, desviacion_estandar, varianza, minimo, maximo):
    with open(nombre + ".csv", "a", newline='') as archivo:
        campos = ['n', 'a', 'esperanza', 'desviacion estandar', 'varianza', 'minimo', 'maximo']
        writer = csv.DictWriter(archivo, fieldnames=campos)
        writer.writerow({'n':n, 'a':a, 'esperanza': esperanza, 'desviacion estandar': desviacion_estandar, 'varianza': varianza, "minimo": minimo, "maximo": maximo})
    pass

def realizar_grafico(lista_n, lista_promedios):
    n = np.array(lista_n)
    promedios = np.array(lista_promedios)
    # cambiar segun n fijo o variable
    log_n = np.log2(n)
    n_fijo = [np.log2(400) for x in range(len(n))]
    # reemplazar en con k = √(n log n) de ser necesario
    plt.plot(n, promedios, "o-", label="k = [√(n log n), n / 2]")
    plt.plot(n, n_fijo, "--", label="log n")
    # plt.xscale('log', base=2)
    # plt.xticks(n, labels=[str(x) for x in n])
    plt.title('Gráfico n vs rondas promedio')
    plt.xlabel("k")
    plt.ylabel("rondas")
    plt.legend()
    plt.grid(False)
    plt.show()

def realizar_grafico_std(lista_x, lista_std, lista_promedios):
    plt.errorbar(lista_x, lista_promedios, yerr=lista_std, fmt='-o', color='blue', ecolor='red', label="k = grado(nodo) / 2; p = log2(n)/n")
    plt.plot(lista_x, [(math.log(int(n), 2)) for n in lista_x], "--", label="log_2 n")
    plt.title('grafico n vs rondas promedio en grafo no completo (C)')
    plt.xlabel("n")
    plt.ylabel("rondas")
    plt.legend()
    plt.show()

def realizar_grafico_variables(lista_x, lista_a, lista_promedio):
    x_val = np.unique(lista_x)
    a_val = np.unique(lista_a)
    print("numpy unique prueba: ", x_val)
    for a in a_val:
        promedio_a = []
        for (nn, aa, rr) in zip(lista_x, lista_a, lista_promedio):
            if aa == a:
                promedio_a.append(rr)
        plt.plot(x_val, promedio_a, "o-", label=f"a = {a}")
    plt.plot(lista_x, [(math.log(int(n), 2)) for n in lista_x], "--", label="log_2 n")
    plt.title('Gráfico n vs rondas promedio en grafo no completo con distintos a')
    plt.xlabel("n")
    plt.ylabel("rondas")
    plt.legend()
    plt.show()
    pass

def realizar_grafico_3d(lista_k, lista_p, lista_promedios):
    lista_k = np.array(lista_k, dtype=float)
    lista_p = np.array(lista_p, dtype=float)
    lista_promedios = np.array(lista_promedios, dtype=float)

    k_vals = np.unique(lista_k)
    p_vals = np.unique(lista_p)
    K, P = np.meshgrid(k_vals, p_vals)

    Z = np.empty_like(K)
    Z[:] = np.nan

    for i, (kk, pp, rr) in enumerate(zip(lista_k, lista_p, lista_promedios)):
        i_k = np.where(k_vals == kk)[0][0]
        i_p = np.where(p_vals == pp)[0][0]
        Z[i_p, i_k] = rr

    fig = plt.figure()
    ax = fig.add_subplot(111, projection='3d')
    surf = ax.plot_surface(K, P, Z, cmap='viridis', edgecolor='none')

    ax.set_title('Superficie 3D: k, p y rondas promedio')
    ax.set_xlabel('k')
    ax.set_ylabel('p')
    ax.set_zlabel('rondas promedio')

    fig.colorbar(surf, ax=ax, shrink=0.5, aspect=10)
    plt.show()

def mostrar_estadisticas(eje_x, lista, lista_std):
    datos = np.array(lista, dtype=float)
    esperanza = np.mean(datos)
    varianza = np.var(datos)
    minimo = np.min(datos)
    maximo = np.max(datos)
    desviacion_estandar = np.std(datos)

    print(f"Esperanza:            {esperanza:.4f}")
    print(f"Desviacion estandar:  {desviacion_estandar:.4f}")
    print(f"Varianza:             {varianza:.4f}")
    print(f"Mínimo:               {minimo:.4f}")
    print(f"Máximo:               {maximo:.4f}")
    guardar_archivo(
        "estadisticas_nvar_er_kalt", eje_x, 
        f"{esperanza:.4f}",
        f"{desviacion_estandar:.4f}",
        f"{varianza:.4f}", 
        f"{minimo:.4f}", 
        f"{maximo:.4f}")
    lista_std.append(desviacion_estandar)

def mostrar_estadisticas_3d(eje_x, eje_y, lista, lista_std):
    datos = np.array(lista, dtype=float)
    esperanza = np.mean(datos)
    varianza = np.var(datos)
    minimo = np.min(datos)
    maximo = np.max(datos)
    desviacion_estandar = np.std(datos)

    print(f"Esperanza:            {esperanza:.4f}")
    print(f"Desviacion estandar:  {desviacion_estandar:.4f}")
    print(f"Varianza:             {varianza:.4f}")
    print(f"Mínimo:               {minimo:.4f}")
    print(f"Máximo:               {maximo:.4f}")
    guardar_archivo_3d(
        "estadisticas_nvar_er_alpha", eje_x, eje_y,
        f"{esperanza:.4f}",
        f"{desviacion_estandar:.4f}",
        f"{varianza:.4f}", 
        f"{minimo:.4f}", 
        f"{maximo:.4f}")
    lista_std.append(desviacion_estandar)

def main1():
    eje_x = []
    lista_std = []
    rondas_n = []
    rondas_promedio = []
    crear_archivo_3d("estadisticas_nvar_er_kalt")
    with open('datos_nvar_er_kalt.csv', 'r') as archivo:
        archivo = csv.reader(archivo)
        lista_lineas = list(archivo)
        eje_x.append(lista_lineas[2][0])
        for i in range(2, len(lista_lineas)):
            if lista_lineas[i][0] not in eje_x:
                eje_x.append(lista_lineas[i][0])
                if len(rondas_n) >= 1:
                    promedio = sum(rondas_n)/len(rondas_n)
                    mostrar_estadisticas(lista_lineas[i - 1][0], rondas_n, lista_std)
                    rondas_promedio.append(promedio)
                    rondas_n = []
            if lista_lineas[i][2]:
                rondas_n.append(float(lista_lineas[i][1]))
        if len(rondas_n) != 0:
            promedio = sum(rondas_n)/len(rondas_n)
            mostrar_estadisticas(lista_lineas[i - 1][0], rondas_n, lista_std)
            rondas_promedio.append(promedio)
            rondas_n = []
    print(eje_x)
    print(rondas_promedio)
    realizar_grafico_std(eje_x, lista_std, rondas_promedio)

def main2():
    eje_x = []
    lista_a = []
    lista_std = []
    rondas_n = []
    rondas_promedio = []
    crear_archivo_3d("estadisticas_nvar_er_alpha")
    with open('datos_nvar_er_alpha.csv', 'r') as archivo:
        archivo = csv.reader(archivo)
        lista_lineas = list(archivo)
        x_actual = None
        a_actual = None
        for linea in lista_lineas[1:]:
            x_val = linea[0]
            a_val = linea[1]
            if (x_val != x_actual or a_val != a_actual) and len(rondas_n) > 1:
                promedio = sum(rondas_n) / len(rondas_n)
                eje_x.append(float(x_actual))
                lista_a.append(float(a_actual))
                mostrar_estadisticas_3d(x_actual, a_actual, rondas_n, lista_std)
                rondas_promedio.append(promedio)
                rondas_n = []
            x_actual = x_val
            a_actual = a_val
            if linea[2]:
                try:
                    rondas_n.append(float(linea[2]))
                except ValueError:
                    pass
        if len(rondas_n) != 0:
            promedio = sum(rondas_n) / len(rondas_n)
            eje_x.append(float(x_actual))
            lista_a.append(float(a_actual))
            mostrar_estadisticas_3d(x_actual, a_actual, rondas_n, lista_std)
            rondas_promedio.append(promedio)
            rondas_n = []
    print(eje_x)
    print(lista_a)
    print(rondas_promedio)
    realizar_grafico_variables(eje_x, lista_a, rondas_promedio)
    pass

def main3():
    eje_x = []
    rondas_n = []
    rondas_promedio = []
    with open('resultados_paralelo_n400.csv', 'r') as archivo:
        archivo = csv.reader(archivo)
        lista_lineas = list(archivo)
        x_actual = None
        for linea in lista_lineas[2:]:
            x_val = linea[0]
            if (x_val != x_actual) and len(rondas_n) > 1:
                promedio = sum(rondas_n) / len(rondas_n)
                eje_x.append(float(x_actual))
                rondas_promedio.append(promedio)
                rondas_n = []
            x_actual = x_val
            if linea[2]:
                try:
                    rondas_n.append(float(linea[1]))
                except ValueError:
                    pass
        if len(rondas_n) != 0:
            promedio = sum(rondas_n) / len(rondas_n)
            eje_x.append(float(x_actual))
            rondas_promedio.append(promedio)
            rondas_n = []
    print(eje_x)
    print(rondas_promedio)
    realizar_grafico(eje_x, rondas_promedio)
    pass

def trash_test():
    with open('estadisticas_nvar_er_kalt.csv', 'r') as archivo:
        print(archivo)
        arch = csv.reader(archivo)
        print(arch)
        print(list(arch))

if __name__ == "__main__":
    main3()