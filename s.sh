#!/bin/bash


if [[ -e "s" ]]
then
  echo "L'exécutable du .c existe"
else
  echo "L'exécutable du .c n'existe pas"
  #$compile_cmd
  gcc -o s s.c
  echo "exécutable crée"
fi
run_cmds="./s"


temps_debut=$(date +%s.%N)
data="data.csv"
temp="temp"
colonne="$temp/colonnes.txt"
ids="$temp/ids.txt"
touch "$colonne"
touch "$ids"
valeurs="$temp/valeurs.txt"

cut -d ';' -f 1,5 "$data" > $colonne
cut -d ';' -f 1 "$data" > $ids
awk '!seen[$0]' $ids > /dev/null
sed -i '1d' "$colonne"
sed -i '1d' "$ids"
while [ -s "$ids" ]; do
	id_trajet=$(head -n 1 "$ids")
	awk -F';' -v id_trajet="$id_trajet" '$1 == id_trajet' "$colonne" > "trajet_$id_trajet.txt"
	trajet="trajet_$id_trajet.txt"
	cut -d ';' -f 2 "$trajet" > "trajet.txt"
	./s "trajet.txt"
	cat "valeurs_temp.txt" >> "$valeurs"
	echo "" >> "$valeurs"
	rm "valeurs_temp.txt"
	rm "trajet_$id_trajet.txt"
	rm "trajet.txt"
	sed -i '1d' "$ids"
done
temps_fin=$(date +%s.%N)
temps_exec=$(echo "$temps_fin - $temps_debut" | bc)
echo "La durée a été de $temps_exec secondes."


