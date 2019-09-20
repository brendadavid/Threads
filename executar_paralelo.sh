#!/bin/bash
echo "Realizando execução paralelo"
for i in {0..9};
do
	echo "Execução " $((i+1))
	java MultiThread > tempo_paralelo_$i
done

echo "Finalizado: tempos de execução paralelo"