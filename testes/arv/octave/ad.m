% Adiciona id com seus filhos, que t�m o n�vel dado, com deslocamento de bits soma
function ad(id, nivel, soma)
	global graus;
	global nums fols;
	global alt;
	
	% Adiciona o pr�prio id
	nums = [nums id];
	% N�o tem filhos
	if nivel > alt
		fols(end+1) = id;
		return;
	end
	for i = 1:graus(nivel)
		ad(bitor(id, bitshift(1, soma+i-1)), nivel+1, soma+graus(nivel));
	end
end