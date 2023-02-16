from .nd import NoiseDistribution, stddevf
from .lwe_parameters import LWEParameters
TiGER128 = LWEParameters(
    n=512,
    q=256,
# RLWR estimate 
    Xs=NoiseDistribution.SparseTernary(512,71,71),
    Xe=NoiseDistribution.Uniform(-1,0), # RLWR stdev=0.5 is smaller than RLWE. so, estimate this.

# RLWE estimate     
#    Xs=NoiseDistribution.SparseTernary(512,55,55), #RLWE
#    Xe=NoiseDistribution.DiscreteGaussian(0.85), # RLWE e_1=HWT(512,32)=stdev 0.25 + c_u=Uniform[-1,1]=stdev 0.82. then e_1+c+u = "0.85"

    m=512,
    tag="TiGER128",
)

TiGER192 = LWEParameters(
    n=1024,
    q=256,
# RLWR estimate
    Xs=NoiseDistribution.SparseTernary(1024,66,66),
    Xe=NoiseDistribution.Uniform(-1,0), # RLWR stdev=0.5 is smaller than RLWE. so, estimate this.

# RLWE estimate
#    Xs=NoiseDistribution.SparseTernary(1024,66,66),
#    Xe=NoiseDistribution.DiscreteGaussian(0.84), # RLWE e_1=HWT(1024,32)=stdev 0.18, c_u=UniformMod(3)=[-1,1]=stdev 0.82. then e_1+c_u = "0.84"

    m=1024,
    tag="TiGER192",
)

TiGER256 = LWEParameters(
    n=1024,
    q=256,
# RLWR estimate
    Xs=NoiseDistribution.SparseTernary(1024,98,98),
    Xe=NoiseDistribution.Uniform(-1,0), # RLWR stdev=0.5 is smaller than RLWE. so, estimate this.

# RLWE estimate   
#    Xs=NoiseDistribution.SparseTernary(1024,98,98), 
#    Xe=NoiseDistribution.DiscreteGaussian(0.53), # RLWE e_1=HWT(1024,32)=stdev 0.18 + c_u=Uniform[-1,0]=stdev 0.5. then e_1 + c_u = "0.53"

    m=1024,
    tag="TiGER256",
)
