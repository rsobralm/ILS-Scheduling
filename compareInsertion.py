import pandas as pd
import numpy as np

table1 = pd.read_csv("resultscabalimited.csv", sep = ',')
table2 = pd.read_csv("testesInsert.csv" , sep = ',')

matrizResults = []
linha = []
instances = []

for i in range(0,10):
    lista = []
    matrizResults.append(lista)


for i in range(0, len(table2["instancia"])):
    for j in range(0, len(table1["instances"])):
        if(table2["instancia"][i] == table1["instances"][j]):
            if(table2["best"][i] <= table1["vbih"][j] and table2["best"][i] <= table1["bs"][j] and table2["best"][i] <= table1["ig"][j]):
                if(i < 46):
                    matrizResults[0].append(table2["instancia"][i])
                if(i >= 46 and i < 92):
                    matrizResults[1].append(table2["instancia"][i])
                if(i >= 92 and i < 139):
                    matrizResults[2].append(table2["instancia"][i])
                if(i >= 139 and i < 186):
                    matrizResults[3].append(table2["instancia"][i])
                if(i >= 186 and i < 232):
                    matrizResults[4].append(table2["instancia"][i])
                if(i >= 232 and i < 278):
                    matrizResults[5].append(table2["instancia"][i])
                if(i >= 278 and i < 324):
                    matrizResults[6].append(table2["instancia"][i])
                if(i >= 324 and i < 370):
                    matrizResults[7].append(table2["instancia"][i])
                if(i >= 370 and i < 416):
                    matrizResults[8].append(table2["instancia"][i])
                if(i >= 416):
                    matrizResults[9].append(table2["instancia"][i])

for i in range (0, 10):
    print("RODANDO COM TAMANHO:", i + 4, " W:", len(matrizResults[i]))
'''    
    for j in range(0, len(matrizResults[i])):
        print(matrizResults[i][j])   '''
               
ganha11 = []
ganha13 = []

for i in range(323,368):
    '''
    print(table2["best"][i])
    print(table2["best"][i+92])
    '''
    if(table2["best"][i] <= table2["best"][i+92]):
        ganha11.append(table2["instancia"][i])
    if(table2["best"][i] >= table2["best"][i+92]):
        ganha13.append(table2["instancia"][i])

    
print("TAMANHO 11 GANHA DE 13 INSERCOES EM", len(ganha11), " INSTANCIAS")
for i in range(0, len(ganha11)):
    print(ganha11[i])

print("TAMANHO 13 GANHA DE 11 INSERCOES EM", len(ganha13), " INSTANCIAS")
for i in range(0, len(ganha13)):
    print(ganha13[i])



#print(table2)