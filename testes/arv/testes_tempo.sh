IFS=$'\n'

# Graus com os quais rodar�
for g in $(cat graus_testes.txt)
do
	IFS=' '
	echo -------
	echo \> ./tmp $* - $g
	./tmp $* - $g
done
