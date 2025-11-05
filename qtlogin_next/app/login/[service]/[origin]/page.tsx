'use client';
import { useParams } from 'next/navigation';
import validOrigins from '@/shared/validOrigins';
import {validServices, servicesEnum} from '@/shared/validServices';
import { useEffect } from 'react';

export default function Home() {
    const params = useParams<{ service: string; origin: string }>()
    const serviceValid = validServices.has(params.service);
    const originValid = validOrigins.has(params.origin);
    const sendResponseToQt = () => {
        const payload = {
            action: "add_item",
            item: {
                id: 42,
                name: "Golden Apple",
                price: 12.5
            }
        };

        // Encode JSON for URL (Base64 or percent)
        const json = encodeURIComponent(JSON.stringify(payload));
        const url = `qtsample://open?data=${json}`;

        // Open the desktop app
        window.location.href = url;
    }

    useEffect(() => {
        
        const proceedGoogleLogin = async () =>{
            console.log("google call");
            const clientId = process.env.NEXT_PUBLIC_GOOGLE_CLIENT_ID; // Replace with your Google client ID
            const redirectUri = encodeURIComponent(`${process.env.NEXT_PUBLIC_BACKEND_URL}/auth/login_google`);
            const scope = encodeURIComponent('openid email profile');
            const state = encodeURIComponent("example");
            const responseType = 'code';

            const authUrl = `https://accounts.google.com/o/oauth2/v2/auth?` +
                            `client_id=${clientId}` +
                            `&redirect_uri=${redirectUri}` +
                            `&response_type=${responseType}` +
                            `&scope=${scope}` +
                            `&state=${state}`;

            window.location.assign(authUrl);
        };

        const proceedGithubLogin = async () =>{
            console.log("github call");
            const clientId = process.env.NEXT_PUBLIC_GITHUB_CLIENT_ID; // Replace with your Google client ID
            

            const authUrl = `https://github.com/login/oauth/authorize?` +
                            `client_id=${clientId}&` +
                            `state={\"bouncer\": \"testing\"}`;

            window.location.assign(authUrl);
        };

        const loginTask = new Map<number, () => void>();
        loginTask.set(servicesEnum.google, proceedGoogleLogin);
        loginTask.set(servicesEnum.github, proceedGithubLogin);

        loginTask.get(validServices.get(params.service)!)?.();

    },[params.service]);

    return (
        <div className="flex flex-col min-h-screen items-center justify-center bg-zinc-50 font-sans dark:bg-black">
            <p>Service: {serviceValid && params.service}</p>
            <p>Origin: {originValid && params.origin}</p>
            <button type="button" style={{'cursor': 'pointer', 'border': '1px solid #fff', 'padding': '4px', 'margin': '4px'}} onClick={()=>sendResponseToQt()} disabled={!(serviceValid && originValid)}>Enviar resposta</button>
        </div>
    );
}