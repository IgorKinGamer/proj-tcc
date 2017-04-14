% Gera todas as m�scaras E poss�veis
function mascs = mascsE(graus)

	% Nenhum ou um bit para cada n�vel (2 ou mais garantem colis�o)
	mascs(1) = bitshift(1, sum(graus)) - 1;
	soma = 0;
	for nivel = 1:length(graus)
		n = length(mascs);
		grau = graus(nivel);
		
		% mascs vira [mascs mascs ...] grau+1 vezes
		for i = 1:grau
			mascs = [mascs mascs(1:n)];
		end
		
		% Cada grupo do antigo mascs (exceto o primeiro, 0)
		for i = 1:grau
			% Cada m�scara do grupo i recebe bit do grupo (soma+i-1)
			pos = (i*n+1):((i+1)*n);
			mascs(pos) = bitxor(mascs(pos), bitshift(1, soma+i-1));
		end
		
		soma += grau;
	end

end

function semi = semiMascsE(graus)

	

end