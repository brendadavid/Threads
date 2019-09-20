#!/bin/bash
echo "Realizando execução sequencial"
for i in {0..9};
do
	echo "Execução " $((i+1))
	java Sequencial > tempo_sequencial_0$i
done

echo "Finalizado: tempos de execução sequencial"