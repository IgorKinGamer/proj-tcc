close all;

% Algoritmos
SMPL = 1;
NOVO = 2;
HWLC = 3;
MTRZ = 4;
ALGS = 4;

sem = dlmread('sem_acesso/A/i10000a1000-1.4.1.1.9.2.1.1.4/resultados0.csv', ',', 1, 0);
com = dlmread('A/i10000a1000-1.4.1.1.9.2.1.1.4/resultados0.csv', ',', 1, 0);

%{
for i = 1:4
	figure
	boxplot({sem(:, i), com(:, i)});
end
%}

sem_e_com = {};
for i = 1:4
	sem_e_com{2*i - 1} = sem(:, i);
	sem_e_com{2*i}     = com(:, i);
end

if true % Com/sem acesso

	figure;
	s = boxplot(sem_e_com);
	axis([.5 8.5]);
	rotulos = {
		"\\shortstack{\\Simples\\\\sem\\\\acesso}"
		"\\shortstack{com\\\\acesso}"
		"\\shortstack{\\Novo\\\\sem\\\\acesso}"
		"\\shortstack{com\\\\acesso}"
		"\\shortstack{\\Hwloc\\\\sem\\\\acesso}"
		"\\shortstack{com\\\\acesso}"
		"\\shortstack{\\Matriz\\\\sem\\\\acesso}"
		"\\shortstack{com\\\\acesso}"
	};
	set(gca, "xtick", 1:8, "xticklabel", rotulos);
	%title("Tempo dos algoritmos");
	xlabel(gca, "Algoritmo");
	ylabel(gca, "Tempo (ms)");
	
	print(gcf, "D:\\box_sem_com.tex", "-dtikz")

	%figure, boxplot(sem);
	%figure, boxplot(com);
	
else % Tempo médio

	% Árvores
	arvs = ...
	{
		"1 4 1 1 9 2 1 1 4"
		"2 1 1 8 1 1 1 2"
		"2 2 1 5 1 1 1 1 8"
		"4 4 1 3 2 1 1 1"
		"8 1 1 6 1 1 1"
	}';
	nArvs = length(arvs);
	% Número de folhas
	nFolhas = ...
	[
		1*4*1*1*9*2*1*1*4
		2*1*1*8*1*1*1*2
		2*2*1*5*1*1*1*1*8
		4*4*1*3*2*1*1*1
		8*1*1*6*1*1*1
	]';
	% Ordena
	[nFolhas inds] = sort(nFolhas);
	arvs = arvs(inds);
	% Número de pares
	nPares = nFolhas .* (nFolhas - 1);


	%%% Carrega os dados %%%
	A = {};
	B = {};
	for i = 1:length(arvs)
		cam = ["/i10000a1000-" strrep(arvs{i}, " ", ".") "/resultados0.csv"];
		A{end+1} = dlmread(["A" cam], ',', 1, 0);
		B{end+1} = dlmread(["B" cam], ',', 1, 0);
	end

	%%% Comparar tempo médio por acesso %%%
	for arv = 1:nArvs % Cada árvore
		nIters = 10000;
		nBuscas = nIters * nPares(arv);
		% Tempo médio (ns)
		for alg = 1:ALGS
			tAcesso{1}(alg, arv) = 10e6 * median(A{arv}(:, alg)) / nBuscas;
			tAcesso{2}(alg, arv) = 10e6 * median(B{arv}(:, alg)) / nBuscas;
		end
	end


	formato = {"or", "sg", "db", "hm", "^c"};
	%axis(1:4);
	figure; figA = gcf; eixoA = gca; hold(eixoA, "on"); axis([.5 ALGS+.5]);
	figure; figB = gcf; eixoB = gca; hold(eixoB, "on"); axis([.5 ALGS+.5]);
	set([eixoA eixoB], "xtick", 1:4, "xticklabel", {"\\Simples" "\\Novo" "\\Hwloc" "\\Matriz"});
	%title(eixoA, "Máquina A");
	%title(eixoB, "Máquina B");
	xlabel(eixoA, "Algoritmo");
	xlabel(eixoB, "Algoritmo");
	ylabel(eixoA, "Tempo médio (ns)");
	ylabel(eixoB, "Tempo médio (ns)");
	for arv = 1:nArvs
		f = [formato{arv} ";" arvs{arv} ";"];
		cor = formato{arv}(2);
		% A
		plot(eixoA, 1:ALGS, tAcesso{1}(:, arv), f, "markerfacecolor", cor);
		% B
		plot(eixoB, 1:ALGS, tAcesso{2}(:, arv), f, "markerfacecolor", cor);
	end

	nNos = [];
	for i = 1:nArvs
		arvore = arvs{i};
		n = 1;
		for pos = length(arvore):-2:1
			g = str2num(arvore(pos));
			n = n*g + 1;
		end
		nNos(end+1) = n;
	end


	% Salva
	print(figA, "D:\\tMedioA.tex", "-dtikz")
	print(figB, "D:\\tMedioB.tex", "-dtikz")

end
